class Line
  def initialize(line)
    @cont = Array.new
    #from_s(line) if line.is_a?(String)
  end

  #def from_s
  #end

  #def to_s
  #end

  def to_a; @cont; end

  def [](index); @cont[index]; end

  #def []=(index, obj); 

  def empty?; @cont.empty?; end

  def size; @cont.size; end
end
