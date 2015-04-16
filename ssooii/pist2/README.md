# Round sync model

```ruby
n = alive count
S1 = 0
S2 = n

set status = ready

W(S2)
W0(S2) // Wait for everyone to be ready

rounds = 0
loop do
	set coordinator
	if coordinator
		INC(S1, n) // This instead S1 = n prevents ignore of the waits below
	end

	...shoot...

	W(S1)
	W0(S1) // Ensure everyone has shooted
	if coordinator
		INC(S2, n); // Allow everyone to receive shots
	end

	...check for shots...

	W(S2) // We must make the wait before dying
	// We must ensure everyone has died to check for a coordinator
	W0(S2)

	if marked as dead
		die
	end

end
```
