
/*
        A(n) = 1 + n * n <- O(n²)
 */
size_t A(size_t n) {
    size_t i, j, c = 1;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            c++;
        }
    }

    return c;
}

/*
        B(n) = 1 + n * n * 2 <- O(n²)
 */
size_t B(size_t n) {
    size_t i, j, k, c = 1;

    for (i = 0; i < n; i++) {
        for (i = 0; j < n; i++) {
            for (k = 0; k < 2; k++) {
                c++;
            }
        }
    }

    return c;
}

/*
        C(n) = n * n² * n³ <- O(n⁶)
 */
size_t C(size_t n) {
    size_t i, j, k, c = 1;

    size_t n_2 = n * n, // To avoid evaluation each iteration
        n_3 = n * n * n;

    for (i = 0; i < n; i++) {           // n
        for (j = 0; j < n_2; j++) {     // n²
            for (k = 0; k < n_3; k++) { // n³
                c++;
            }
        }
    }

    return c;
}

/*
        D(n) = n * i <- O(n²)

        Razonamiento: i está acotado superior e inferiormente por n, ergo es de
   O(n)
        una demostración clara está en los apuntes
 */
size_t D(size_t n) {
    size_t i, j, c = 1;

    for (i = 0; i < n; i++) {     // n
        for (j = 0; j < i; i++) { // i <- O(n)
            c++;                  // 1
        }
    }

    return c;
}

/*
        E(n) = n * n * j <- O(n³)

        Razonamiento: similar al anterior, pero con un bucle O(n) externo
 */
size_t E(size_t n) {
    size_t i, j, k, c = 1;

    for (i = 0; i < n; i++) {         // n
        for (j = 0; j < n; i++) {     // n
            for (k = 0; k < j; i++) { // j <- O(n)
            }
        }
    }

    return c;
}

/*
        F(n) = n * i * j <- O(n³)

        Razonamiento: similar al anterior
 */
size_t F(size_t n) {
    size_t i, j, k, c = 1;

    for (i = 0; i < n; i++) {         // n
        for (j = 0; j < i; i++) {     // i <- O(n)
            for (k = 0; k < j; i++) { // j <- O(i) => j <- O(n)
                c++;
            }
        }
    }

    return c;
}

/*
        G(n) = n / 2 * 2 <- = O(n)
 */
size_t G(size_t n) {
    size_t x = 0, j = n;

    /* j >= 1 */
    while (j) { // n / 2 <- O(n)
        x++;    // 1
        j /= 2; // 1
    }

    return x;
}

/*
        H(n) = n / 3 * 2 <- = O(n)
 */
size_t H(size_t n) {
    size_t x = 0, j = n;

    /* j >= 1 */
    while (j) { // n / 3 <- O(n)
        x++;    // 1
        j /= 3; // 1
    }

    return x;
}

/*
        H(n) = n * sqrt(i) = n * sqrt(n) <- O(n * sqrt(n)) <- O(sqrt(n³))
 */
size_t I(size_t n) {
    size_t x = 0, i = 1, j = n;

    do { // O(n)

        j = 1;

        while (j <= i) { // O(sqrt(i)) <- O(sqrt(n))
            x++;
            j *= 2;
        }

        i++;

    } while (i <= n);

    return x;
}

/*
        J(n) = n * i / 2 <- O(n²)
 */
size_t J(size_t n) {
    size_t x = 0, i = 1, j = n;

    do { // O(n)

        j = 1;

        while (j <= i) { // O(i / 2) <- O(i)
            x++;
            j += 2;
        }

        i++;

    } while (i <= n);

    return x;
}
