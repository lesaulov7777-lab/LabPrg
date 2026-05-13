defmodule Cycle do
  def cycle(s, t) do
    if String.length(s) != String.length(t) do
      false
    else
      String.contains?(s <> s, t)
    end
  end
end

# Примеры использования
IO.puts(Cycle.cycle("waterbottle", "erbottlewat")) # true
IO.puts(Cycle.cycle("abc", "acb"))               # false
IO.puts(Cycle.cycle("ngtu", "ngtu"))             # true
