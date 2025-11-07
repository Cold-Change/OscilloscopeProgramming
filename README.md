# Oscilloscope & Function Generator System

Command-line application simulating an oscilloscope and function generator.

## Features

**Oscilloscope:** Start/stop acquisition, collect samples, read/write files, wait/delay  
**Function Generator:** Generate SINE, SQUARE, TRIANGLE, SAWTOOTH waveforms with configurable frequency, amplitude, offset

---

## Compilation

```bash
cd functionGenerator
g++ -std=c++11 -Iinclude src/*.cpp -o oscilloscope
./oscilloscope --help
```

---

## Command Reference

### Oscilloscope Commands

| Command | Parameters | Example |
|---------|-----------|---------|
| `START_SCOPE` | None | `START_SCOPE` |
| `STOP_SCOPE` | None | `STOP_SCOPE` |
| `COLLECT_SAMPLES` | `numberOfSamples=N` | `COLLECT_SAMPLES numberOfSamples=10000` |
| `READ_FILE` | `filename=<path>` | `READ_FILE filename=input.dat` |
| `WRITE_FILE` | `filename=<path>` | `WRITE_FILE filename=output.dat` |

### Function Generator Commands

| Command | Parameters | Example |
|---------|-----------|---------|
| `START_FGEN` | None | `START_FGEN` |
| `STOP_FGEN` | None | `STOP_FGEN` |
| `GENERATE_WAVEFORM` | None | `GENERATE_WAVEFORM` |
| `SET_WAVE_TYPE` | `type=<SINE\|SQUARE\|TRIANGLE\|SAWTOOTH>` | `SET_WAVE_TYPE type=SINE` |
| `SET_FREQUENCY` | `value=<Hz>` | `SET_FREQUENCY value=1000` |
| `SET_AMPLITUDE` | `value=<Volts>` | `SET_AMPLITUDE value=2.5` |
| `SET_OFFSET` | `value=<Volts>` | `SET_OFFSET value=0.5` |
| `SET_NUM_SAMPLES` | `value=<N>` | `SET_NUM_SAMPLES value=5000` |
| `SET_OUTPUT_FILE` | `filename=<path>` | `SET_OUTPUT_FILE filename=wave.dat` |

### Utility Commands

| Command | Parameters | Example |
|---------|-----------|---------|
| `WAIT` | `seconds=N` | `WAIT seconds=5` |

---

## Usage Examples

### Oscilloscope

```bash
# Basic: Collect and save
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=10000 WRITE_FILE filename=data.dat STOP_SCOPE

# With delay
./oscilloscope START_SCOPE WAIT seconds=2 COLLECT_SAMPLES numberOfSamples=5000 WRITE_FILE filename=delayed.dat STOP_SCOPE

# Multiple collections (accumulates data)
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=1000 COLLECT_SAMPLES numberOfSamples=2000 WRITE_FILE filename=combined.dat STOP_SCOPE
```

### Function Generator

```bash
# Default SINE wave
./oscilloscope START_FGEN SET_OUTPUT_FILE filename=sine.dat GENERATE_WAVEFORM STOP_FGEN

# SQUARE wave
./oscilloscope START_FGEN SET_WAVE_TYPE type=SQUARE SET_OUTPUT_FILE filename=square.dat GENERATE_WAVEFORM STOP_FGEN

# Custom SINE (440 Hz, 2.5V amplitude)
./oscilloscope START_FGEN SET_WAVE_TYPE type=SINE SET_FREQUENCY value=440 SET_AMPLITUDE value=2.5 SET_OUTPUT_FILE filename=a440.dat GENERATE_WAVEFORM STOP_FGEN

# High-resolution waveform
./oscilloscope START_FGEN SET_NUM_SAMPLES value=10000 SET_OUTPUT_FILE filename=highres.dat GENERATE_WAVEFORM STOP_FGEN

# Waveform with DC offset
./oscilloscope START_FGEN SET_WAVE_TYPE type=TRIANGLE SET_AMPLITUDE value=2.0 SET_OFFSET value=1.0 SET_OUTPUT_FILE filename=offset.dat GENERATE_WAVEFORM STOP_FGEN
```

### File-Based Commands

Create `commands.txt`:
```
START_SCOPE
COLLECT_SAMPLES numberOfSamples=10000
WRITE_FILE filename=test_output.dat
STOP_SCOPE

START_FGEN
SET_WAVE_TYPE type=SINE
SET_OUTPUT_FILE filename=sine_wave.dat
GENERATE_WAVEFORM
STOP_FGEN
```

Run: `./oscilloscope -f commands.txt`

---

## Testing Guide

### Quick Tests

**Test 1: Basic Oscilloscope**
```bash
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=1000 WRITE_FILE filename=test1.dat STOP_SCOPE
# Expected: Creates test1.dat (1000 bytes)
```

**Test 2: All Waveforms**
```bash
./oscilloscope START_FGEN SET_WAVE_TYPE type=SINE SET_OUTPUT_FILE filename=sine.dat GENERATE_WAVEFORM STOP_FGEN
./oscilloscope START_FGEN SET_WAVE_TYPE type=SQUARE SET_OUTPUT_FILE filename=square.dat GENERATE_WAVEFORM STOP_FGEN
./oscilloscope START_FGEN SET_WAVE_TYPE type=TRIANGLE SET_OUTPUT_FILE filename=triangle.dat GENERATE_WAVEFORM STOP_FGEN
./oscilloscope START_FGEN SET_WAVE_TYPE type=SAWTOOTH SET_OUTPUT_FILE filename=sawtooth.dat GENERATE_WAVEFORM STOP_FGEN
# Expected: 4 files, each 1000 bytes (default)
```

**Test 3: File Operations**
```bash
# Create source
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=2000 WRITE_FILE filename=source.dat STOP_SCOPE

# Copy file
./oscilloscope START_SCOPE READ_FILE filename=source.dat WRITE_FILE filename=copy.dat STOP_SCOPE

# Verify identical
diff source.dat copy.dat
```

**Test 4: Wait Command**
```bash
./oscilloscope START_SCOPE WAIT seconds=3 COLLECT_SAMPLES numberOfSamples=1000 STOP_SCOPE
# Expected: 3-second pause before collection
```

---

## Troubleshooting

**"Command validation failed"**  
Missing required parameter. Example fix:
```bash
# Wrong: ./oscilloscope COLLECT_SAMPLES
# Correct: ./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=1000 STOP_SCOPE
```

**"Cannot collect samples. Oscilloscope not running"**  
Use `START_SCOPE` before `COLLECT_SAMPLES`

**"Cannot generate. Function generator not running"**  
Use `START_FGEN` before `GENERATE_WAVEFORM`

**"No data to write"**  
Collect or read data before using `WRITE_FILE`

**Files not created**  
Check current directory (`pwd`) or use absolute paths

**Compilation errors**  
Verify all files exist: `ls include/*.h src/*.cpp`

---