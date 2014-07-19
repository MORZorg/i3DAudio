#!/usr/bin/env ruby

# @brief Reads HRTF data from a file and writes it to a C++ header file.
# @author Zucchelli Maurizio
# @version 0.0.2
# @date 2014-07-15

class Array
  def odd_values
    self.select.each_with_index { |_, i| i.odd? }
  end
  def even_values
    self.select.each_with_index { |_, i| i.even? }
  end
end

class DataReader
  attr_reader :values, :coordinates

  def initialize()  
    @values = Array.new
    @coordinates = Array.new
  end  

  def read_data(filename)
    File.open(filename, 'r').each do |line|
      if /(?<elevation>-?[[:digit:]]+)e(?<angle>[[:digit:]]+)a/ =~ line
        @coordinates.push([elevation.to_i, angle.to_i])
      else
        filter = line.scan(/-?[[:digit:]]+\.[[:digit:]]+[[:space:]]/)
        if filter.length > 0
          @values.push(filter.map{ |i| i.to_f })
        end
      end
    end

    self
  end

  def elaborate_data()
    File.open("data_reader.m", 'w') do |file|
      file.write("input = [\n");
      coordinates.each_with_index do |point, i|
        cartesian = [ Math.cos(point[0]) * Math.cos(point[1]),
                      Math.cos(point[0]) * Math.sin(point[1]),
                      Math.sin(point[0]) ]

        file.write("\t%f, %f, %f;\n" % cartesian )
      end
      file.write("];")
    end

    `matlab -nodisplay -nosplash -r "data_triangulation; exit" > /dev/null`

    `rm data_reader.m`

    self
  end

  def write_data(filename)
    File.open(filename, 'w') do |file|

      # Header
      file.write("#ifndef __HRTF_DATA__\n#define __HRTF_DATA__\n\n#include \"m_pd.h\"\n\n")

      # Values
      file.write("extern \"C\" t_float hrtf_impulses[][2][#{values[0].length/2}] =\n{\n")
      values.each_with_index do |point, i|
        file.write("\t{\n\t\t{\n")

        point.even_values.each_with_index do |value, j|
          if j%8 == 0
            file.write("\t\t\t")
          else
            file.write(" ")
          end

          file.write("%f," % value)

          if j%8 == 7
            file.write("\n")
          end
        end

        file.write("\t\t},\n\t\t{\n")

        point.odd_values.each_with_index do |value, j|
          if j%8 == 0
            file.write("\t\t\t")
          else
            file.write(" ")
          end

          file.write("%f," % value)

          if j%8 == 7
            file.write("\n")
          end
        end

        file.write("\t\t}\n\t}")

        if i < values.length-1
          file.write(",");
        end
        file.write("\n");
      end
      file.write("};\n\n")

      # Coordinates
      file.write("extern \"C\" t_int hrtf_coordinates[][2] =\n{\n")
      coordinates.each_with_index do |point, i|
        file.write("\t{ %d, %d }" % point )

        if i < coordinates.length-1
          file.write(",")
        end
        file.write("\n")
      end

      file.write("};\n\n")
    end

    elaborate_data

    File.open(filename, 'a') do |file|
      # Footer
      file.write("#endif  // __HRTF_DATA__");
    end
  end
end

DataReader.new.read_data(ARGV[0]).write_data("hrtf_data.hpp")
