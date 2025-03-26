# Digital Biquad Filters

This repository contains a collection of digital biquad filters implemented in
Python.

The filters are based off the C++ implementation, which can be found [here](https://github.com/alex-parisi/biquad-filters/tree/main/cpp).

---

### Brief:

For information on biquad filters, you can check out my
website [here](https://atparisi.com/html/digitalBiquadFilter.html).

---

### Usage:

To use any of the filters, just create an instance of the filter and process 
your data:

```python
from biquads import LowPassFilter
import numpy as np

data_in = np.array([0.1, 0.2, 0.3, 0.4, 0.5])

lpf = LowPassFilter.create(
    cutoff=1000.0,
    sample_rate=44100,
    q_factor=0.707
)

if lpf is not None:
    data_out = lpf.process(data)
```

---

### Supported Filters:

- Generic Digital Biquad
- Low Pass
- High Pass
- Band Pass
- Notch
- All Pass
- Peaking EQ
- Low Shelf
- High Shelf