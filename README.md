# Oscilloscope & Function Generator System

Real-time data acquisition and waveform generation system using FTDI USB interface chips with multi-threaded operation.

## Features

**Oscilloscope Mode**
- Real-time digital signal acquisition via FTDI GPIO pins (DB0-DB7)
- Multi-threaded data collection with live progress display
- Configurable sampling rates (300 baud to 3 MHz)
- Binary data storage and hex dump visualization
- Single-threaded and multi-threaded collection modes

**Function Generator Mode**
- Generate SINE, SQUARE, TRIANGLE, and SAWTOOTH waveforms
- Configurable frequency, amplitude, and DC offset
- Variable sample count (100-100,000 samples)
- Direct waveform file output

**Hardware Support**
- FTDI FT232R, FT245R, FT2232H, FT232H chipsets
- Asynchronous Bit Bang mode for GPIO input
- 8-bit parallel data acquisition (DB0-DB7)
- File-only mode (works without FTDI hardware)

---

## Hardware Setup

### FTDI Pin Configuration (Input Mode)

| Pin Name | Physical Pin | Function | Voltage |
|----------|-------------|----------|---------|
| **DB0** | 10 | Data Bit 0 (LSB) | 0-5V |
| **DB1** | 9  | Data Bit 1 | 0-5V |
| **DB2** | 8  | Data Bit 2 | 0-5V |
| **DB3** | 7  | Data Bit 3 | 0-5V |
| **DB4** | 6  | Data Bit 4 | 0-5V |
| **DB5** | 5  | Data Bit 5 | 0-5V |
| **DB6** | 4  | Data Bit 6 | 0-5V |
| **DB7** | 3  | Data Bit 7 (MSB) | 0-5V |
| **GND** | 1, 21, 25 | Ground | 0V |
| **VCC** | 20 | Power Supply | 5V |

### Reading Digital States

Each byte read represents the state of all 8 pins:
```
Example: 0x01 = 00000001 (binary)
         │││││││└─ DB0: HIGH (1)
         ││││││└── DB1: LOW  (0)
         │││││└─── DB2: LOW  (0)
         ││││└──── DB3: LOW  (0)
         │││└───── DB4: LOW  (0)
         ││└────── DB5: LOW  (0)
         │└─────── DB6: LOW  (0)
         └──────── DB7: LOW  (0)
```

**Common Values:**
- `0x00` = All pins LOW (no signal/GND connected)
- `0x01` = Only DB0 HIGH (VCC on pin 10)
- `0xFF` = All pins HIGH (VCC on all data pins)
- `0x55` = Alternating pattern (01010101)

### Simple Test Circuit
```
Test 1: Single Pin
VCC (Pin 20) ───┬─── DB0 (Pin 10)
                └─── GND (Pin 1) via 10kΩ resistor

Expected reading: 0x01

Test 2: Multiple Pins
VCC (Pin 20) ───┬─── DB0 (Pin 10)
                ├─── DB1 (Pin 9)
                └─── DB2 (Pin 8)

Expected reading: 0x07 (binary: 00000111)
```

---

## Compilation
```bash
# macOS
g++ -std=c++17 -Iinclude src/*.cpp lib/macos/libftd2xx.a -framework CoreFoundation -framework IOKit -o oscilloscope

# Linux
g++ -std=c++17 -Iinclude src/*.cpp -lftd2xx -lpthread -o oscilloscope

# Windows (MinGW)
g++ -std=c++17 -Iinclude src/*.cpp lib/windows/ftd2xx.lib -o oscilloscope.exe
```

---

## Command Reference

### Oscilloscope Commands

| Command | Parameters | Description |
|---------|-----------|-------------|
| `START_SCOPE` | None | Initialize oscilloscope |
| `STOP_SCOPE` | None | Stop oscilloscope |
| `COLLECT_SAMPLES` | `numberOfSamples=N` | Single-threaded collection |
| `COLLECT_SAMPLES_THREADED` | `waitSeconds=N` | Multi-threaded timed collection |
| `READ_FILE` | `filename=<path>` | Load data from file |
| `WRITE_FILE` | `filename=<path>` (optional) | Save data (default: output.dat) |
| `SET_BAUD_RATE` | `baudrate=<baud>` | Set FTDI baud rate (300-3000000) |
| `SET_SAMPLE_FREQUENCY` | `frequency=<Hz>` | Set sampling frequency |

### Function Generator Commands

| Command | Parameters | Description |
|---------|-----------|-------------|
| `START_FGEN` | None | Initialize function generator |
| `STOP_FGEN` | None | Stop function generator |
| `GENERATE_WAVEFORM` | None | Generate configured waveform |
| `SET_WAVE_TYPE` | `type=<SINE\|SQUARE\|TRIANGLE\|SAWTOOTH>` | Select waveform |
| `SET_FREQUENCY` | `value=<Hz>` | Set waveform frequency |
| `SET_AMPLITUDE` | `value=<Volts>` | Set waveform amplitude |
| `SET_OFFSET` | `value=<Volts>` | Set DC offset |
| `SET_NUM_SAMPLES` | `value=<N>` | Set samples per waveform |
| `SET_OUTPUT_FILE` | `filename=<path>` | Set output filename |

### Utility Commands

| Command | Parameters | Description |
|---------|-----------|-------------|
| `WAIT` | `seconds=N` | Pause execution |

---

## Usage Examples

### Basic Hardware Data Acquisition
```bash
# Collect 1000 samples from FTDI pins
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=1000 WRITE_FILE filename=signal.dat STOP_SCOPE

# View data in hex format
hexdump -C signal.dat | head -20
```

### Multi-threaded Collection with Timer
```bash
# Collect data for 10 seconds with real-time progress
./oscilloscope START_SCOPE COLLECT_SAMPLES_THREADED waitSeconds=10 WRITE_FILE filename=timed.dat STOP_SCOPE
```

### Using Default Output File
```bash
# Writes to output.dat automatically
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=5000 WRITE_FILE STOP_SCOPE
```

### High-Speed Sampling
```bash
# Set 115200 baud (1.8 MHz effective sampling)
./oscilloscope START_SCOPE SET_BAUD_RATE baudrate=115200 COLLECT_SAMPLES numberOfSamples=10000 WRITE_FILE filename=highspeed.dat STOP_SCOPE
```

### Generate Test Waveforms
```bash
# SINE wave (1000 Hz, 2.5V amplitude)
./oscilloscope START_FGEN SET_WAVE_TYPE type=SINE SET_FREQUENCY value=1000 SET_AMPLITUDE value=2.5 SET_OUTPUT_FILE filename=sine.dat GENERATE_WAVEFORM STOP_FGEN

# SQUARE wave
./oscilloscope START_FGEN SET_WAVE_TYPE type=SQUARE SET_NUM_SAMPLES value=5000 SET_OUTPUT_FILE filename=square.dat GENERATE_WAVEFORM STOP_FGEN

# TRIANGLE with DC offset
./oscilloscope START_FGEN SET_WAVE_TYPE type=TRIANGLE SET_AMPLITUDE value=2.0 SET_OFFSET value=1.0 SET_OUTPUT_FILE filename=triangle.dat GENERATE_WAVEFORM STOP_FGEN
```

### File Operations
```bash
# Read existing data file
./oscilloscope START_SCOPE READ_FILE filename=input.dat WRITE_FILE filename=output.dat STOP_SCOPE

# Compare files
diff input.dat output.dat
```

### Command File Execution

Create `test.txt`:
```
START_SCOPE
COLLECT_SAMPLES numberOfSamples=1000
WRITE_FILE filename=batch_test.dat
STOP_SCOPE
```

Execute:
```bash
./oscilloscope -f test.txt
```

---

## Hardware Test Procedures

### Test 1: Verify FTDI Detection
```bash
./oscilloscope --help
# Should show device info without errors
```

### Test 2: Floating Pin Test (No Connection)
```bash
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=100 WRITE_FILE filename=floating.dat STOP_SCOPE
hexdump -C floating.dat | head -5
```
**Expected**: Random or 0x00 values (floating pins)

### Test 3: Single Pin HIGH (VCC on DB0)
**Hardware**: Connect VCC (pin 20) to DB0 (pin 10)
```bash
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=100 WRITE_FILE filename=db0_high.dat STOP_SCOPE
hexdump -C db0_high.dat | head -5
```
**Expected**: `0x01 0x01 0x01...` (binary: 00000001)

### Test 4: Multiple Pins HIGH
**Hardware**: Connect VCC to DB0, DB1, DB2
```bash
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=100 WRITE_FILE filename=multi_high.dat STOP_SCOPE
hexdump -C multi_high.dat | head -5
```
**Expected**: `0x07 0x07 0x07...` (binary: 00000111)

### Test 5: All Pins HIGH
**Hardware**: Connect VCC to all data pins (DB0-DB7)
```bash
./oscilloscope START_SCOPE COLLECT_SAMPLES numberOfSamples=100 WRITE_FILE filename=all_high.dat STOP_SCOPE
hexdump -C all_high.dat | head -5
```
**Expected**: `0xFF 0xFF 0xFF...` (binary: 11111111)

### Test 6: Multi-threaded Collection
```bash
./oscilloscope START_SCOPE COLLECT_SAMPLES_THREADED waitSeconds=5 WRITE_FILE filename=threaded.dat STOP_SCOPE
```
**Expected**: Live progress bar showing sample rate and countdown

---

## Sample Output

### Successful Data Collection
```
[scpController] Collecting 1000 samples...
[scpReader] 1000 samples read from FTDI device

========== Data Preview (First 64 samples) ==========
Offset    Hex Values                                      ASCII
--------  ------------------------------------------------  ----------------
00000000  01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01  ................
00000010  01 01 01 01 01 01 01 01 01 01 01 01 01 01 01 01  ................
...

=== Data Statistics ===
Total samples: 1000
Unique values: 1

Top 5 most common values:
  0x01 (1): 1000 times (100.0%)
=======================

[scpWriter] ✓ 1000 bytes written to file: signal.dat
```

### Multi-threaded Collection
```
========== Multi-threaded Data Collection ==========
[scpController] Duration: 10 seconds
[scpController] Sample Rate: 1000 Hz
====================================================

[Timer] 9 s remaining
[DataCollector] Samples:     142 | Last: 0x01 (1) | [==>                 ] 1s/10s | Rate: 142.0 Hz
[Timer] 8 s remaining
[DataCollector] Samples:     284 | Last: 0x01 (1) | [====>               ] 2s/10s | Rate: 142.0 Hz
...
[Timer] Time expired
[DataCollector] Collection complete. Total samples: 1402

========== Collection Summary ==========
[scpController] Samples collected: 1402
[scpController] Total time: 10024 ms
[scpController] Average rate: 139.9 Hz
========================================
```

---

## Data Analysis

### View Hex Dump
```bash
hexdump -C signal.dat | head -20
```

### Count Unique Values
```bash
xxd -p signal.dat | fold -w2 | sort | uniq -c | sort -rn
```

### Extract Statistics
```bash
# Total bytes
wc -c < signal.dat

# First 10 values (decimal)
xxd -p signal.dat | fold -w2 | head -10 | while read hex; do echo $((16#$hex)); done
```

### Compare Files
```bash
# Binary comparison
cmp signal1.dat signal2.dat

# Detailed diff
diff <(hexdump -C signal1.dat) <(hexdump -C signal2.dat)
```

---

## Troubleshooting

### Problem: "No FTDI devices connected"
**Solution**: 
1. Check USB connection
2. Verify FTDI drivers installed: `ls /usr/local/lib | grep ftd2xx`
3. Check device permissions: `sudo chmod 666 /dev/cu.usbserial-*`

### Problem: All readings are 0x00
**Cause**: Pins are floating (not connected to anything)  
**Solution**: Connect test signal (VCC or GND) to at least one pin

### Problem: Readings don't change when connecting VCC
**Cause**: Wrong pin or poor connection  
**Solution**: 
1. Verify pin numbers (DB0 = pin 10, VCC = pin 20, GND = pin 1)
2. Check continuity with multimeter
3. Ensure solid connection (no breadboard contact issues)

### Problem: "Command validation failed for WRITE_FILE"
**Solution**: Use `WRITE_FILE` without parameters for default output file, or add `filename=output.dat`
---

## Pin Voltage Reference

| Value Read | Binary | Pin States | Example Connection |
|-----------|--------|------------|-------------------|
| 0x00 | 00000000 | All LOW | All pins to GND |
| 0x01 | 00000001 | DB0 HIGH | VCC to DB0 only |
| 0x03 | 00000011 | DB0,DB1 HIGH | VCC to DB0,DB1 |
| 0x0F | 00001111 | DB0-DB3 HIGH | VCC to DB0-DB3 |
| 0x55 | 01010101 | Alternating | VCC to DB0,DB2,DB4,DB6 |
| 0xAA | 10101010 | Alternating | VCC to DB1,DB3,DB5,DB7 |
| 0xFF | 11111111 | All HIGH | VCC to all pins |

---

## Performance Notes

- **Default Baud Rate**: 9600 (153.6 kHz effective sampling)
- **Maximum Baud Rate**: 3,000,000 (48 MHz effective sampling)
- **Typical Sampling**: ~140-160 Hz in multi-threaded mode (limited by USB latency)
- **File I/O**: Works without FTDI hardware (file-only mode)

---