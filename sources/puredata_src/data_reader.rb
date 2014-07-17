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

  def write_data(filename)
    File.open(filename, 'w') do |file|

      # Header
      file.write("#include \"m_pd.h\"\n\n")
      
      # Values
      file.write("extern \"C\" t_float hrtf_impulses[#{values.length}][2][#{values[0].length/2}] =\n{\n")
      values.each_with_index do |point, i|
        file.write("\t{\n\t\t{\n")

        point.even_values.each_with_index do |value, j|
          if j%8 == 0
            file.write("\t\t\t")
          else
            file.write(" ")
          end

          file.write("%+f," % value)

          if j%8 == 7
            file.write("\n")
          # else
          #   file.write(",")
          end
        end

        file.write("\t\t},\n\t\t{\n")

        point.odd_values.each_with_index do |value, j|
          if j%8 == 0
            file.write("\t\t\t")
          else
            file.write(" ")
          end

          file.write("%+f," % value)

          if j%8 == 7
            file.write("\n")
          # else
          #   file.write(",")
          end
        end

        file.write("\t\t}\n\t}")

        if i < values.length-1
          file.write(",");
        end
		file.write("\n");
      end
      file.write("};")

      file.write("\n\n")

      # Coordinates
      file.write("extern \"C\" t_int hrtf_coordinates[#{coordinates.length}][2] =\n{\n")
      coordinates.each_with_index do |point, i|
        # file.write("\t\t{ #{point[0]}, #{point[1]} }")
        file.write("\t{ %+03d, %+04d }" % point )

        if i < coordinates.length-1
          file.write(",")
        end
        file.write("\n")
      end
      
      file.write("};")
    end
  end
end

DataReader.new.read_data(ARGV[0]).write_data("hrtf_data.hpp")
