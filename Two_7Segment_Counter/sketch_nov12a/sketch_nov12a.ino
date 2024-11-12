// Define pin connections to the shift register
const int dataPin = A0;   // SER on 74HC595
const int clockPin = A2;  // SRCLK on 74HC595
const int latchPin = A1;  // RCLK on 74HC595
const int buttonPin = 2;  // Button to start the countdown
const int pauseBtn = 3;   // Button to pause countdown at 10

// Bit patterns for displaying digits 0-9 on a common anode seven-segment display
byte numbers[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

int count = 30;         // Start countdown from 30
bool countdownStarted = false; // Flag to track if countdown has started

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin with internal pull-up resistor
  pinMode(pauseBtn, INPUT_PULLUP);  // Set pause button pin with internal pull-up resistor

  Serial.begin(9600); // Optional for debugging

  displayCount(count); // Display "30" initially at startup
}

void loop() {
  // Check if button is pressed to start the countdown
  if (digitalRead(buttonPin) == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(buttonPin) == LOW) { // Confirm button press after debounce
      countdownStarted = true; // Start the countdown
    }
  }

  // Run the countdown only if countdownStarted is true
  if (countdownStarted) {
    // Start countdown from the current count
    for (int currentCount = count; currentCount >= 0; currentCount--) {
      displayCount(currentCount); // Display countdown number
      delay(1000); // Wait for a second between counts

      // Check if the pause button is held when the countdown is at or below 10
      if (currentCount < 10) {
        while (digitalRead(pauseBtn) == LOW) {
          displayCount(10); // Display 10 while the pause button is held
          currentCount = 10; // Pause at 10
          delay(100); // Small delay to debounce
        }
      }

      // Check if the pause button is held at countdown 10
      if (currentCount == 10 && digitalRead(pauseBtn) == LOW) {
        // If button is held, stop at 10
        while (digitalRead(pauseBtn) == LOW) {
          displayCount(10); // Display 10 continuously while paused
          delay(100); // Small delay to debounce
        }
        continue; // Continue countdown after button is released
      }
    }

    // After countdown finishes, reset to 30 and stop countdown
    count = 30; 
    countdownStarted = false;
    displayCount(count); // Display "30" again after countdown ends
  }
}

// Function to display a two-digit number on a two-digit seven-segment display
void displayCount(int number) {
  int tens = number / 10;  // Extract tens digit
  int ones = number % 10;  // Extract ones digit

  digitalWrite(latchPin, LOW);                 // Begin latch
  
  // Send the bit pattern for the tens digit
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[tens]);
  
  // Send the bit pattern for the ones digit
  shiftOut(dataPin, clockPin, MSBFIRST, numbers[ones]);
  
  digitalWrite(latchPin, HIGH);                // Complete the transfer to display
}
