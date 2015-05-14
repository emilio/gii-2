# Synchronisation model

```
S0 = 0
S1 = 0
S2 = 0
```

```
main() {

    for i in 0..n {
        spawn_thread()
    }

    for i in 0..n {
        WAIT(S0)
    }

    INC(S2, n)

    wait_for_threads();

    return last_alive_thread;

}
```


```
child() {
    SIGNAL(S0)
    WAIT(S2)

    loop {
        if im_coordinator() {
            if im_last_alive() {
                let last_alive_thread = current_thread_id();
                return;
            }
        }
        shoot();

        SIGNAL(S0);
        if im_coordinator() {
            for i in 0..alive_count {
                WAIT(S0);
            }
            INC(S1, alive_count);
        }
        WAIT(S1);

        if received_shots() {
            mark_as_dead();
        }

        SIGNAL(S0);
        if im_coordinator() {
            for i in 0..alive_count {
                WAIT(S0);
            }
            INC(S2, alive_count);
        }
        WAIT(S2);

        if marked_as_dead() {
            return;
        }
    }
}
```

