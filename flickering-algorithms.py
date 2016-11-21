from matplotlib import pyplot


class Lsfr:
    """Represents an 8 bit linear shift register."""
    def __init__(self):
        self._r = 1
    
    def seed(self, v):
        self._r = (v if v != 0 else 1) & 0xFF
    
    def get_seed(self):
        return self._r

    def next(self):
        bits = ((self._r >> 0) ^ (self._r >> 2) ^ (self._r >> 3) ^ (self._r >> 4) & 1) & 0xFF
        self._r = ((self._r >> 1) | (bits << 7)) & 0xFF 
        return self._r

class FlickerFlourescent:
    def __init__(self):
        self.lsfr = Lsfr()

    def __iter__(self):
        cycle = 255
        prev = 0
        target = 64

        while True:
            #k = self.lsfr.next()

            if prev < target:
                prev += 1 
            elif prev > target:
                prev -= 1 

            cycle += 1
            if cycle > 238:
                target = self.lsfr.next() ^prev
                cycle = 0

            yield prev

class FlickerIncandescent:
    def __init__(self):
        self.lsfr = Lsfr()

    def __iter__(self):
        cycle = 255
        prev = 0
        target = 255

        while True:
            #k = self.lsfr.next()

            if prev < target:
                prev = (prev + 1) & 0xFF
            elif prev > target:
                prev = (prev - 1) & 0xFF 

            cycle += 1
            if cycle > 238:
                k = self.lsfr.next()
                if k % 11 == 0:
                    target = (k >> 5)
                else:
                    target = 0xFF
                cycle = 0

            yield prev

def main():
    xvals = [x for x in range(0, 255*32)]

    ff = FlickerFlourescent()
    ffi = iter(ff)

    fi = FlickerIncandescent()
    fii = iter(fi)

    ff_yvals = [next(ffi) for i in xvals]
    fi_yvals = [next(fii) for i in xvals]

    pyplot.plot(
        xvals, ff_yvals, "-r",
        xvals, fi_yvals, "-g",
        xvals, [(255 if x % 255 == 0 else 0) for x in xvals], "|b--")
    pyplot.show()

if __name__ == '__main__':
    import sys
    sys.exit(int(main() or 0))
