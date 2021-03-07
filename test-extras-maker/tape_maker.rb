#!/usr/bin/ruby

class TapeMaker
    FRAME_TICKS = 71680
    FRAME_MICROS = 20000

    MILLIS_MICROS = 1000
    SECOND_MICROS = 1000 * MILLIS_MICROS

    def self.micros_to_ticks(ticks)
        return ticks * FRAME_TICKS / FRAME_MICROS
    end

    def self.ticks_to_micros(ticks)
        return ticks * FRAME_MICROS / FRAME_TICKS
    end

    FLAG_HEADER = 0
    FLAG_DATA = 255

    TYPE_PROGRAM = 0
    TYPE_NUMBER_ARRAY = 1
    TYPE_CHARACTER_ARRAY = 2
    TYPE_BYTES = 3

    PILOT_PULSE_TICKS = 2168 # about 605 micros
    PILOT_HEADER_PULSES = 8063 # about 2250 micros
    PILOT_DATA_PULSES = 3223 # about 900 micros
    SYNC_PULSE_FIRST_TICKS = 667 # about 186 micros
    SYNC_PULSE_SECOND_TICKS = 735 # about 205 micros
    BIT_ZERO_PULSE_TICKS = 855 # about 239 micros
    BIT_ONE_PULSE_TICKS = 1710 # about 477 micros
    DELAY_FIRST_TICKS = micros_to_ticks(MILLIS_MICROS) # 3584 ticks, 1000 micros
    DELAY_SECOND_TICKS = micros_to_ticks(SECOND_MICROS - MILLIS_MICROS) # 3580416 ticks, 999000 micros

    def initialize
        @chunks = []
    end

    def save_tap(file_name)
        @data = []

        @chunks.each do |chunk|
            push_uint16(chunk[:data].size + 2)
            push_uint8(chunk[:flag])
            @data += chunk[:data]
            push_uint8(chunk[:checksum])
        end

        save_data(file_name)
    end

    def save_pulses(file_name)
        @data = []
        @pulses = []
        @total_ticks = 0
        is_first_pilot = true

        puts "Pulses"

        @chunks.each_with_index do |chunk, index|
            @pulse_bit = false # start every pilot with low bit
            puts " - Chunk #{index + 1}"

            @block_ticks = 0
            push_pilot_pulses(chunk[:flag], is_first_pilot)
            puts '   - Pilot:    %-8d | %-8d' % [@block_ticks, @total_ticks]

            @block_ticks = 0
            push_sync_pulses
            puts '   - Sync:     %-8d | %-8d' % [@block_ticks, @total_ticks]

            @block_ticks = 0
            push_value_pulses(chunk[:flag])
            puts '   - Flag:     %-8d | %-8d' % [@block_ticks, @total_ticks]

            @block_ticks = 0
            chunk[:data].each { |v| push_value_pulses(v) }
            puts '   - Data:     %-8d | %-8d' % [@block_ticks, @total_ticks]

            @block_ticks = 0
            push_value_pulses(chunk[:checksum])
            puts '   - Checksum: %-8d | %-8d' % [@block_ticks, @total_ticks]

            @block_ticks = 0
            is_first_pilot = false

            unless index + 1 == @chunks.size
                ticks_add = PILOT_PULSE_TICKS
                msg_append = ', pilot pulse'
            else
                ticks_add = 0
                msg_append = ''
            end

            if @pulse_bit
                push_pulse(DELAY_FIRST_TICKS)
                puts '   - Delay:    %-8d | %-8d | first' % [@block_ticks, @total_ticks]

                @block_ticks = 0
                push_pulse(DELAY_SECOND_TICKS + ticks_add)
                puts '   - Delay:    %-8d | %-8d | second%s' % [@block_ticks, @total_ticks, msg_append]
            else
                push_pulse(DELAY_FIRST_TICKS + DELAY_SECOND_TICKS + ticks_add)
                puts '   - Delay:    %-8d | %-8d | first, second%s' % [@block_ticks, @total_ticks, msg_append]
            end
        end

        puts '  - Micros: %d' % [TapeMaker.ticks_to_micros(@total_ticks)]
        save_data(file_name)
    end

    def save_wav(file_name, num_channels, bits_per_sample, sample_rate)
        puts 'WAV'
        puts '  - Channels: %d' % [num_channels]
        puts '  - BPS:      %d' % [bits_per_sample]
        puts '  - Rate:     %d' % [sample_rate]

        @data = []
        last_samples = 0
        current_ticks = 0

        @pulses.each do |pulse|
            current_ticks += pulse[:ticks]
            current_samples = TapeMaker.ticks_to_micros(current_ticks) * sample_rate / SECOND_MICROS
            pulse_samples = current_samples - last_samples
            last_samples = current_samples

            value = pulse[:bit] ? 96 : -32

            if bits_per_sample == 16
                value *= 0x100
            elsif bits_per_sample == 24
                value *= 0x10000
            elsif bits_per_sample == 32
                value *= 0x1000000
            end

            (num_channels * pulse_samples).times do
                if bits_per_sample == 8
                    push_sint8(value)
                elsif bits_per_sample == 16
                    push_sint16(value)
                elsif bits_per_sample == 24
                    push_sint24(value)
                elsif bits_per_sample == 32
                    push_sint32(value)
                end
            end
        end

        inner_data = @data
        @data = []

        push_uint32(0x46464952) # RIFF id: "RIFF"
        push_uint32(4 + 24 + 8 + inner_data.size) # RIFF size: RIFF type, FMT, DATA id, DATA size, wav data
        push_uint32(0x45564157) # RIFF type: "WAVE"

        push_uint32(0x20746D66) # FMT id: "fmt "
        push_uint32(16) # FMT size
        push_uint16(1) # FMT audio format: PCM
        push_uint16(num_channels) # FMT num channels
        push_uint32(sample_rate) # FMT sample rate
        push_uint32(num_channels * sample_rate * (bits_per_sample / 8)) # FMT byte rate
        push_uint16(num_channels * (bits_per_sample / 8)) # FMT block align
        push_uint16(bits_per_sample) # FMT bits per sample

        push_uint32(0x61746164) # DATA id: "data"
        push_uint32(inner_data.size) # DATA size
        @data += inner_data

        save_data(file_name)
    end

    # name
    #   max 10 characters
    # param_1
    #   for TYPE_PROGRAM -- autostart line number (or 32768 if no autostart)
    #   for TYPE_BYTES -- start of code block
    def append_chunk(type, name, param_1, content)
        content_bytes = content.bytes
        param_2 = (type == TYPE_PROGRAM ? content_bytes.size : 32768)

        @chunks << compute_checksum({
            :flag => FLAG_HEADER,
            :data => [type] + ('%-10.10s' % name).bytes + [
                content_bytes.size % 256,
                content_bytes.size / 256,
                param_1 % 256,
                param_1 / 256,
                param_2 % 256,
                param_2 / 256,
            ],
        })

        @chunks << compute_checksum({
            :flag => FLAG_DATA,
            :data => content_bytes,
        })
    end

    def compute_checksum(chunk)
        checksum = chunk[:flag]
        chunk[:data].each { |v| checksum ^= v }
        chunk[:checksum] = checksum

        return chunk
    end

    def push_sint8(value)
        value = value.to_i

        raise Error.new("#{value} > 0x7F") if value > 0x7F
        raise Error.new("#{value} < -0x80") if value < -0x80

        @data << (value & 0xFF)
    end

    def push_sint16(value)
        value = value.to_i

        raise Error.new("#{value} > 0x7FFF") if value > 0x7FFF
        raise Error.new("#{value} < -0x8000") if value < -0x8000

        value &= 0xFFFF
        @data << (value & 0xFF)
        @data << ((value >> 8) & 0xFF)
    end

    def push_sint24(value)
        value = value.to_i

        puts "#{value} > 0x7FFFFF" if value > 0x7FFFFF
        puts "#{value} < -0x800000" if value < -0x800000

        value &= 0xFFFFFF
        @data << (value & 0xFF)
        @data << ((value >> 8) & 0xFF)
        @data << ((value >> 16) & 0xFF)
    end

    def push_sint32(value)
        value = value.to_i

        raise Error.new("#{value} > 0x7FFFFFFF") if value > 0x7FFFFFFF
        raise Error.new("#{value} < -0x80000000") if value < -0x80000000

        value &= 0xFFFFFFFF
        @data << (value & 0xFF)
        @data << ((value >> 8) & 0xFF)
        @data << ((value >> 16) & 0xFF)
        @data << ((value >> 24) & 0xFF)
    end

    def push_uint8(value)
        value = value.to_i
        raise Error.new("#{value} > 0xFF") unless (value >> 8) == 0

        @data << (value & 0xFF)
    end

    def push_uint16(value)
        value = value.to_i
        raise Error.new("#{value} > 0xFFFF") unless (value >> 16) == 0

        @data << (value & 0xFF)
        @data << ((value >> 8) & 0xFF)
    end

    def push_uint32(value)
        value = value.to_i
        raise Error.new("#{value} > 0xFFFFFFFF") unless (value >> 32) == 0

        @data << (value & 0xFF)
        @data << ((value >> 8) & 0xFF)
        @data << ((value >> 16) & 0xFF)
        @data << ((value >> 24) & 0xFF)
    end

    def push_pulse(ticks)
        push_uint32(ticks)
        @pulses << { :bit => @pulse_bit, :ticks => ticks }
        @pulse_bit = !@pulse_bit

        @block_ticks += ticks
        @total_ticks += ticks
    end

    def push_pilot_pulses(flag, is_first_pilot)
        pulses = (flag == FLAG_HEADER ? PILOT_HEADER_PULSES : PILOT_DATA_PULSES)

        unless is_first_pilot
            pulses -= 1
            @pulse_bit = !@pulse_bit
        end

        pulses.times.each do
            push_pulse(PILOT_PULSE_TICKS)
        end
    end

    def push_sync_pulses
        push_pulse(SYNC_PULSE_FIRST_TICKS)
        push_pulse(SYNC_PULSE_SECOND_TICKS)
    end

    def push_value_pulses(value)
        mask = 0x80

        while mask != 0
            ticks = ((value & mask) != 0) ? BIT_ONE_PULSE_TICKS : BIT_ZERO_PULSE_TICKS

            push_pulse(ticks)
            push_pulse(ticks)

            mask = mask / 2
        end
    end

    def save_data(file_name)
        File.open(file_name, 'wb') do |fo|
            fo << @data.map { |v| v.chr }.join
        end
    end
end

def process
    tm = TapeMaker.new

    tm.append_chunk(TapeMaker::TYPE_PROGRAM, 'Program', 0, '10 REM Cracked by Bill Gilbert')
    tm.append_chunk(TapeMaker::TYPE_BYTES, 'Bytes', 32768, 'This is da best bytes in the world')

    tm.save_tap('../test-extras/tape-1.tap')
    tm.save_pulses('../test-extras/tape-1.pulses')

    tm.save_wav('../test-extras/tape-1-c4b8s44100.wav', 4, 8, 44100)
    tm.save_wav('../test-extras/tape-1-c2b16s44100.wav', 2, 16, 44100)
    tm.save_wav('../test-extras/tape-1-c3b24s22050.wav', 3, 24, 22050)
    tm.save_wav('../test-extras/tape-1-c1b32s22050.wav', 1, 32, 22050)

    puts 'Done'
end

process
