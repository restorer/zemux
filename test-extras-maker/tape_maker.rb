#!/usr/bin/ruby

# 17834152

class TapeMaker
    FRAME_TICKS = 71680
    FRAME_MICROS = 20000

    MILLIS_MICROS = 1000
    SECOND_MICROS = 1000 * MILLIS_MICROS

    def self.ticks_to_micros(ticks)
        return ticks * FRAME_MICROS / FRAME_TICKS
    end

    FLAG_HEADER = 0
    FLAG_DATA = 255

    TYPE_PROGRAM = 0
    TYPE_NUMBER_ARRAY = 1
    TYPE_CHARACTER_ARRAY = 2
    TYPE_BYTES = 3

    PILOT_PULSE_MICROS = ticks_to_micros(2168) # 604 ms
    PILOT_HEADER_PULSES = 8063
    PILOT_DATA_PULSES = 3223
    SYNC_PULSE_FIRST_MICROS = ticks_to_micros(667) # 186 ms
    SYNC_PULSE_SECOND_MICROS = ticks_to_micros(735) # 205 ms
    BIT_ZERO_PULSE_MICROS = ticks_to_micros(855) # 238 ms
    BIT_ONE_PULSE_MICROS = ticks_to_micros(1710) # 477 ms
    DELAY_FIRST_MICROS = MILLIS_MICROS # 1000 ms
    DELAY_SECOND_MICROS = SECOND_MICROS - MILLIS_MICROS # 999000 ms

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
        @pulse_bit = false

        @chunks.each do |chunk|
            push_pilot_pulses(chunk[:flag])

            push_value_pulses(chunk[:flag])
            chunk[:data].each { |v| push_value_pulses(v) }
            push_value_pulses(chunk[:checksum])

            if @pulse_bit
                push_pulse(DELAY_FIRST_MICROS)
                push_pulse(DELAY_SECOND_MICROS)
            else
                push_pulse(DELAY_FIRST_MICROS + DELAY_SECOND_MICROS)
            end
        end

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

    def push_pulse(micros)
        push_uint32(micros)
        @pulse_bit = !@pulse_bit
    end

    def push_pilot_pulses(flag)
        (flag == FLAG_HEADER ? PILOT_HEADER_PULSES : PILOT_DATA_PULSES).times.each do
            push_pulse(PILOT_PULSE_MICROS)
        end

        push_pulse(SYNC_PULSE_FIRST_MICROS)
        push_pulse(SYNC_PULSE_SECOND_MICROS)
    end

    def push_value_pulses(value)
        mask = 0x80

        while mask != 0
            micros = ((value & mask) != 0) ? BIT_ONE_PULSE_MICROS : BIT_ZERO_PULSE_MICROS

            push_pulse(micros)
            push_pulse(micros)

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
