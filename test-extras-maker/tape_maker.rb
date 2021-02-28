#!/usr/bin/ruby

# 63992484 * 20000 / 71680 = 17855045.758929

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
            @data << chunk[:flag]
            @data += chunk[:data]
            @data << chunk[:checksum]
        end

        save_data(file_name)
    end

    def save_pulses(file_name)
        @data = []
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

    def push_uint16(value)
        value = value.to_i

        @data << (value & 0xFF)
        @data << ((value >> 8) & 0xFF)

        puts "#{value} > 0xFFFF" unless (value >> 16) == 0
    end

    def push_uint32(value)
        value = value.to_i

        @data << (value & 0xFF)
        @data << ((value >> 8) & 0xFF)
        @data << ((value >> 16) & 0xFF)
        @data << ((value >> 24) & 0xFF)

        puts "#{value} > 0xFFFFFFFF" unless (value >> 32) == 0
    end

    def push_pulse(ticks)
        push_uint32(ticks)
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

    tm.append_chunk(TapeMaker::TYPE_PROGRAM, 'A', 0, 'a')
    tm.append_chunk(TapeMaker::TYPE_BYTES, 'B', 32768, 'b')

    tm.save_tap('../test-extras/tape.tap')
    tm.save_pulses('../test-extras/tape.pulses')

    puts 'Done'
end

process
