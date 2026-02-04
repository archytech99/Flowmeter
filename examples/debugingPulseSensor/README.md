# Implementation

> YF‑DN50 (_the black plastic_), the most common spec `F(Hz) = 0.2 × Q(L/min)`, which is `~12 pulses per liter`. [walmart.com](https://www.walmart.com/ip/3-5-24Vdc-G2-YF-DN50-Plastic-Hall-Turbine-Water-Meter-NPN-Pulse-Output-Measurement-Flow-Sensor/8318069926)

## Recommended starting values (for your library)

### Your code uses

```cfg
lps = freq / calibrationFactor
So calibrationFactor is Hz per (L/s).
```

1. If the sensor is `F = 0.2 × Q(L/min)`:

    ```cfg
    1 L/s = 60 L/min
    f = 0.2 × 60 = 12 Hz

    ✅ Start with:
    fw.begin(12.0f, 0.0f);
    ```

2. If your readings are off and you suspect the `0.45×Q` variant, try:

    ```cfg
    fw.begin(27.0f, 0.0f);
    ```

### Tolerance

1. Your tolerance only reduces readings. I recommend:

   * Start with `0.0f`
   * If you consistently over‑read by `~3–5%`, set tolerancePercent to that value
   * If you under‑read, do not use tolerance—adjust the calibration factor instead

2. Simple calibration step (fast)

   * Measure a known volume (_e.g., 10.00 L_).
   * Read `fw.getTotalLiters()` (_e.g., 10.6 L_).
   * Update calibration:

     ```cfg
     newCal = oldCal * (measured / actual)
     ```

### Example

```cfg
newCal = 12.0 * (10.6 / 10.0) = 12.72
```
