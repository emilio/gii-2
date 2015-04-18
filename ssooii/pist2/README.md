# Main sync model
```ruby
n = alive count

+---------------------+
|       parent        |
+---------------------+

block_signals()
create_resources()
init_library()

let S1 = 0;
let S2 = n + 1;

for i in 0..n
    create_child()
end

unblock_signals()

W(S2)
W0(S2)

while there_are_alive_children
    last_dead = wait()
end

if alive_count == 1
    return last_dead
end

return 0

+--------------------+
|      children      |
+--------------------+

init_library()
mark_as_ready()
W(S2)
W0(S2)

loop do
    round()
end
```

# Round sync model

```ruby
let im_coordinator : bool = check_coordinator()
if im_coordinator
    if alive_count == 1
        die()
    end
    INC(S1, n) // This instead S1 = n prevents ignore of the waits below
end

shoot()

W(S1)
W0(S1)

if im_coordinator
    INC(S2, n);
end

if shoots_received()
    mark_as_dead()
    decrement_atomically(alive_count)
end

W(S2)
W0(S2)

if marked_as_dead()
    die()
end
```
