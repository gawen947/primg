# primg
Convert 2-bits images in their nearest equivalent prime.

---

Inspired by [this reddit post](https://www.reddit.com/r/math/comments/7qpfls/does_there_exist_a_prime_number_whose).
The principle is very simple. It takes in argument a PBM image and converts it
into a large integer. From there it seeks the nearest upper prime which is
reconverted into a PBM image and shown on the output. Most likely only a few
lesser significant bits would be toggled to reach the next prime and the
pattern in the image would stay the same.

It works well for images up to 128x128 pixels. Beyond that finding the next
prime gets incredibly difficult. Behind the scene it uses the [GMP library](https://gmplib.org)
for bit twiddling and playing with primes. The rest of it is done by hand.

### TODO

Still far from perfect, there are things to fix or implement.
Patches are welcome.

  * The program generates an error if the nearest upper prime is larger
    than the original number, including its leading zeros. For now the
    leading zeros are used to catch the extra bits of the resulting prime.
  * Only the the PBM ASCII format is supported. It would be nice if we
    could use binary PBM too. It is really easy to implement.
  * The current output format is PBM ASCII, but we could also select between
    binary PBM, or decimal.
  * A manpage.
  * See FIXME and TODO in the code.
