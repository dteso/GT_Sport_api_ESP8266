/* loadingScreen: Muestra una sucesión de puntos simulando LOADING */
void loadingScreen() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64 / Adress 0x·C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  display.clearDisplay(); //for Clearing the display
  display.drawBitmap(0, 0, medusaka_logoBitmap, 128, 32, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.display();

// If our prefer to display a progresive dots loader
//  display.clearDisplay();

// Draw a single pixel in white
//  display.setTextSize(3);
//  display.drawPixel(20, 10, WHITE);
//  display.display();
//  delay(500);
//  display.drawPixel(45, 10, WHITE);
//  display.display();
//  delay(500);
//  display.drawPixel(70, 10, WHITE);
//  display.display();
//  delay(500);
//  display.drawPixel(95, 10, WHITE);
//  display.display();
//  delay(500);
}


/*  displayPrint: Imprime un texto text en la posición especificada
    por cursorX y cursorY con un textSize de 1-3 */
void displayPrint(int textSize, int cursorX, int cursorY, String text) {
  display.setTextColor(WHITE);
  display.setTextSize(textSize);
  display.setCursor(cursorX, cursorY);
  display.print(text);
  display.display();
  //delay(500);
}


void testscrolltext(String text) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  display.println(text);
  display.display();      // Show initial text
  delay(100);
  display.startscrollleft(0x00, 0x0F);
}
