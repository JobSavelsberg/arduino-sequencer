#include <Arduino.h>
#include "core/application.h"

// Global application instance
Application app;

void setup()
{
  // Initialize the application
  if (!app.initialize())
  {
    // Handle initialization error
    // For now, just halt - in a production system, you might want error recovery
    while (true)
    {
      delay(1000);
    }
  }
}

void loop()
{
  // Update the application - all logic is now handled within the Application class
  app.update();
}
