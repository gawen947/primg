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

### Dependency

  * [GMP library](https://gmplib.org)
  * [libgawen](https://github.com/gawen947/libgawen)

### TODO

Still far from perfect, there are things to fix or implement.
Patches are welcome.

  * For now the program only seeks for the nearest upper prime to the image.
    So it will generally only modify the last line, unless the image is mostly
    filled. It would be nice if we could hide the "primificaton" as most as
    possible in the image pattern. Obviously the bottom right pixel would always
    be filled. But I guess we can do better than what we have now.
  * Can we go faster? We seek the nearest prime. But if we know there is a
    probable prime a bit further up the road that doesn't affect the image pattern,
    we can check this one instead.
  * Only the PBM ASCII format is supported. It would be nice if we
    could use binary PBM too. It is really easy to implement.
  * The current output format is PBM ASCII, but we could also select between
    binary PBM, or decimal.
  * A manpage.
  * See others FIXME and TODO in the code.
