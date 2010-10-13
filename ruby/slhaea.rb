class Line
  include Enumerable

  def initialize(line)
    @cont = Array.new
    from_s(line)
  end

  def from_s(line)
    clear
    line = line.to_s
    stripped_line = line[/^.*$/].rstrip
    return self if stripped_line.empty?
  end

  def to_s
  end

  def to_a
    @cont
  end

  def [](index)
    @cont[index]
  end

  def each(block)
    @cont.each(block)
  end

  def size
    @cont.size
  end

  def empty?
    @cont.empty?
  end

  def clear
    @cont.clear
  end
end

# vim: sw=2 tw=78
