# Prototype Infrared Remote

### 1. Initialize

```bash
pio init --ide vscode
```

### 2. Upload

```bash
pio run -t upload
pio run -t uploadfs
```

### Reset

```bash
# Format & local dev libs
pio run -t reset
pio run -t clean
```

## Log

### 1. Initialized PlatformIO

```bash
pio project init --board esp32dev --project-option="framework=arduino"
# IntelliSense libraries c_cpp
pio init --ide vscode
```

### 2. Installed Dependencies

```bash
pio lib install z3t0/IRremote
pio init --ide vscode # Resolve deps
```

### 3. Load to Board

```bash
pio device list

# Upload & compile
pio run -t upload
pio device monitor
```

### 4. Compile LittleFS

```bash
pio run -t uploadfs
```
