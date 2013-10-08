#!/usr/bin/env ruby

# Num nodes
nn = ARGV[0].to_i

# Num edges
ne = ARGV[1].to_i

# Output
output = ARGV[2]

nodes = {}
nodes[0] = []

# Build the random Spanning Tree
1.upto(nn-1).each do |n|
  connect_to        = rand(n)
  nodes[n]          = [connect_to]
  nodes[connect_to] << n
end

# We have already added node-1 edges
idx = nn - 1

# Add random nodes
while idx < ne do
  from = rand(nn)
  to   = rand(nn)

  next if nodes[from].include? to
  next if from == to

  idx += 1

  nodes[from] << to
  nodes[to] << from
end

puts "Finished building the graph.\nNow saving the result ..."

# Repetition
repetition = [nn * nn, 100000].min
max        = nn - 1
min        = 0
edge_count = ne
arr        = nodes

# Save the graph
f = File.open(output, "w+")
f.write("#{repetition}\n")
f.write("#{max-min+1} #{edge_count}\n")
min.upto(max).each do |i|
  str = []
  min.upto(max).each do |j|
    arr[i] = arr[i] || []
    if arr[i].include? j
      str.push(1)
    else
      str.push(0)
    end
  end
  f.write("#{str.join(" ")}\n")
end
f.close()
