# Round sync model

```ruby
S1 = S2 = 0
rounds = 0
n = alive count
loop do
	set coordinator
	if coordinator
		INC(S1, n*2) // This instead S1 = 2 * n prevents ignore of the waits below
	end
	// We make two waits: one for preventing early start and one to mark the end
	W(S1)

	...shoot...

	W(S1)
	if coordinator
		W0(S1) // Ensure everyone has shooted
		INC(S2, n*2); // Allow everyone to receive shots
	end
	// Same strategy here
	W(S2)

	...check for shots...

	W(S2) // We must make the wait before dying

	if marked as dead
		die
	end

	// This W0 is global, not as the previous one
	// We must ensure everyone has died to check for a coordinator
	W0(S2)
end
```


