## Settings
- Adds panel to control custom window settings to default settings page
- Creates signal to open custom window

## CustomGUI
- Creates a window that holds status labels and control widgets
- Creates signal to close *CustomWindow* and return to settings
- Connects *CustomWindow::update* to the uiState update signal
- Creates sidebar of button to exit window and toggle visibility of status widgets
- Creates various custom widgets and organizes layout
