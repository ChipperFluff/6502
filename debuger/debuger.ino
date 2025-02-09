const char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 400, 42, 44, 46, 48, 50, 52};
const char DATA[] = {27, 29, 31, 33, 35, 37, 39, 41, 43};
const int CLOCK_PIN = 23; // Pin for clock signal
const int SIGNAL_PIN = 13; // Pin for signaling step execution
const int READ_WRITE = 25; // Pin to determine read/write mode
const int STEP_DELAY = 500; // Fixed delay for auto-step mode in milliseconds

bool autoStep = false; // Flag for automatic stepping mode

void setup() {
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(SIGNAL_PIN, OUTPUT);
  pinMode(READ_WRITE, INPUT);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(SIGNAL_PIN, LOW);
  
  // Configure address lines as input
  for (int n = 0; n < 16; n += 1) {
    pinMode(ADDR[n], INPUT);
  }
  // Configure data lines as input
  for (int n = 0; n < 9; n += 1) {
    pinMode(DATA[n], INPUT);
  }
  Serial.begin(57600); // Initialize serial communication
}

void loop() {
  if (!isStep()) { return; }
  clockPulse();
  
  char output[32];
  unsigned int address = 0;
  // Read address from input pins
  for (int n = 0; n < 16; n += 1) {
    int bit = digitalRead(ADDR[n]) ? 1 : 0;
    Serial.print(bit);
    address = (address << 1) + bit;
  }

  Serial.print("  -  ");

  unsigned int data = 0;
  // Read data from input pins
  for (int n = 0; n < 9; n += 1) {
    int bit = digitalRead(DATA[n]) ? 1 : 0;
    Serial.print(bit);
    data = (data << 1) + bit;
  }

  // Determine mode (Read or Write)
  char mode = digitalRead(READ_WRITE) ? 'R' : 'W';
  
  // Format and print output
  sprintf(output, "    %04x    %c %02x", address, mode, data);
  Serial.println(output);
}

// Generate clock pulse for step execution
void clockPulse() {
  digitalWrite(CLOCK_PIN, HIGH);
  digitalWrite(SIGNAL_PIN, HIGH);
  delay(100);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(SIGNAL_PIN, LOW);
  delay(50);
}

// Check if step command is received or auto mode is active
bool isStep() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.equals("start")) {
      autoStep = true;
      return false;
    }
    if (command.equals("stop")) {
      autoStep = false;
      return false;
    }
    if (command.equals("step")) {
      return true;
    }
  }
  
  // Auto step mode enabled
  if (autoStep) {
    delay(STEP_DELAY);
    return true;
  }
  return false;
}
