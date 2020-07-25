#!/usr/bin/ruby

class TapeMaker
    FLAG_HEADER = 0
    FLAG_DATA = 255

    TYPE_PROGRAM = 0
    TYPE_NUMBER_ARRAY = 1
    TYPE_CHARACTER_ARRAY = 2
    TYPE_BYTES = 3

    def initialize
        @chunks = []
    end

    # name
    #   max 10 characters
    # param_1
    #   for TYPE_PROGRAM -- autostart line number (or 32768 if no autostart)
    #   for TYPE_BYTES -- start of code block
    def append_chunk(type, name, param_1, content)
        content_bytes = content.bytes
        param_2 = (type == TYPE_PROGRAM ? content_bytes.size : 32768)

        @chunks << {
            :flag => FLAG_HEADER,
            :data => [type] + ('%-10.10s' % name).bytes + [
                content_bytes.size % 256,
                content_bytes.size / 256,
                param_1 % 256,
                param_1 / 256,
                param_2 % 256,
                param_2 / 256,
            ],
        }

        @chunks << {
            :flag => FLAG_DATA,
            :data => content_bytes,
        }
    end

    def save_tap(file_name)
        data = []

        @chunks.each do |chunk|
            size = chunk[:data].size + 2

            data << size % 256
            data << size / 256
            data << chunk[:flag]
            data += chunk[:data]

            checksum = chunk[:flag]
            chunk[:data].each { |v| checksum ^= v }

            data << checksum
        end

        File.open(file_name, 'wb') do |fo|
            fo << data.map { |v| v.chr }.join
        end
    end
end

tm = TapeMaker.new
tm.append_chunk(TapeMaker::TYPE_PROGRAM, 'A', 0, 'a')
tm.append_chunk(TapeMaker::TYPE_BYTES, 'B', 32768, 'b')
tm.save_tap('../test-extras/tape.tap')

puts 'Done'
