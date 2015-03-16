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
		INC(S2, n*2); // Allow everyone to receive shots
	end
	// Same strategy here, but this lock prevents them to reach the bottom before the coordinator has set the value
	W(S2)

	...check for shots...

	W(S2) // We must make the wait before dying

	if marked as dead
		die
	end

	// We must ensure everyone has died to check for a coordinator
	W0(S2)
end
```


