
Download Videos from video folder for the Demo 

💉 t:slim X2 Insulin Pump Simulator 
A full-featured Qt/C++ system simulation of a modern insulin pump, designed to emulate real-world behavior, safety logic, and insulin delivery workflows. 

📋 Table of Contents 
1. Project Overview 
2. Core Features 
3. Directory & File Structure 
4. Key Components & Class Descriptions 
5. Build and Run Instructions 
6. Usage Instructions 
7. System Design Principles


1. Project Overview 

This project is a realistic insulin pump simulator modeled after the Tandem t:slim X2 pump. It mimics the actual user experience of managing diabetes through CGM data, insulin boluses, and automated control algorithms.

The purpose of this simulator is: 
- To simulate real-time glucose readings. 
- To allow for manual and automated insulin delivery. 
- To ensure safety through warning checks (low BG, insulin, battery). 
- To provide interactive user profiles with editable insulin parameters. 
- To track all activity via a comprehensive event history log. 

2. Core Features 

Real-Time CGM Simulation:	       Background simulation of blood glucose using a timer (1 sec = 5 minutes sim time). 
Manual & Extended Bolus Delivery:Users can enter carbs, BG, and IOB for calculated insulin dose. 
Control IQ Automation:	         Automated insulin delivery or suspension based on projected glucose. 
Warning System:	                 Monitors insulin level less than 4U, battery if battary less than 5% , and glucose every 30 seconds. 
User Profiles:                   Create, edit, and delete. 
History Tracking:	               Logs every bolus, BG reading, and warning alert with timestamps. 

3. Directory & File Structure 

├── main.cpp                 # Entry point of the Qt application

├── MainWindow.h/.cpp        # Top-level GUI window, sets up layout & starts modules   

├── BolusDialog.h/.cpp       # A dialog containing the BolusDeliveryWidget  

├── BolusDeliveryWidget.h/.cpp 

│   └── Allows manual/extended bolus entry & calculations 

├── CGMGraphWidget.h/.cpp    # Real-time BG graph using QChart 
  
├── CgmSimulator.h/.cpp      # Generates BG readings periodically (1s = 5 sim minutes) 

├── PumpController.h/.cpp    # Mediates manual bolus logic, logs CGM data, runs Control IQ 

├── WarningChecker.h/.cpp    # Checks battery/insulin/BG every 30s, logs & shows warnings 

├── AlertDialog.h/.cpp       # Table view of Warning records 

├── HistoryDialog.h/.cpp     # Shows a table of all events (manual/auto bolus, CGM, warnings) 

├── HistoryManager.h/.cpp    # Stores all HistoryRecord objects in a list 

├── HistoryRecord.h          # Struct with time, type, insulin amount, notes 

├── BolusSafetyManager.h/.cpp 

│   └── Checks constraints for bolus delivery (max single, daily limit, cooldown) 

├── ProfileDialog.h/.cpp     # Allows CRUD operations on User Profiles 

├── UserProfileManager.h/.cpp 

│   └── Holds multiple UserProfile objects & active profile 

├── UserProfile.h            # Data structure with basalRate, carbRatio, etc. 

├── icons.qrc                # Qt resource file listing icon assets 

├── MainWindow.ui            # (Optional) UI form (if used in Qt Designer) 

├── PumpSimulation.pro       # Qt project file: lists sources, headers, resources 

└── (Assorted .ui or resource icons) 

4. Key Components & Class Descriptions: 

🔷 MainWindow 
- Central hub that creates the layout (battery/insulin bars, navigation, graph). 
- Connects major modules like PumpController, CgmSimulator, etc. 
- Opens modal dialogs for bolus, profile, history, and alerts. 

🔷 PumpController 
- Mediator pattern that routes logic between bolus entry, CGM data, and insulin safety. 
- Implements Control IQ logic to predict BG 30 mins ahead and act accordingly. 
- Records insulin events through the HistoryManager. 

🔷 CgmSimulator 
- Timer-based simulation that updates BG every second (5 simulated minutes). 
- Uses random walk or sinusoidal algorithm to emulate real glucose variability. 
- Emits bgUpdated(double) signal to any subscriber (graph, controller). 

🔷 BolusDeliveryWidget 
- Allows input of carbs, insulin on board (IOB), and BG. 
- Offers manual BG or CGM BG options. 
- Supports extended bolus delivery (portion over time). 
- Performs insulin dose suggestion using formula: 
- Suggested Dose = (Carbs / CarbRatio) + (BG - Target) / CorrectionFactor - IOB.
  
🔷 BolusSafetyManager 
Ensures: 
- Max single bolus not exceeded. 
- Max daily total not breached. 
- Cooldown time (e.g. 30 min) respected. 
- Works with user profile values. 

🔷 UserProfileManager / UserProfile 
- Handles multiple user configurations with: 
- carbRatio, correctionFactor, targetBG, basalRate 
- Custom thresholds for low/high BG 

🔷 WarningChecker 
- Timer runs every 30 seconds to check: 
- Battery level < 5% 
- Insulin reservoir < 4 units 
- BG too high (> 14.0 mmol/L) or too low (< 3.9 mmol/L) 
- Creates WarningRecord and triggers pop-up dialog. 

🔷 HistoryManager / HistoryDialog 
- Logs all major actions: CGM readings, bolus events, warnings. 
- Each event contains timestamp, type, insulin amount, source (manual/auto). 
- Filterable by date or event type in HistoryDialog. 

🔷 CGMGraphWidget 
- Live BG graph using QChartView (1h, 3h, 6h ranges). 
- Smooth animations and dynamic axes. 


5. Build and Run Instructions 

📌 Using Qt Creator (Recommended) 
1. Open TandemInsulinPumpSimulator.pro 
2. Press Build then Run 

🧪 Using Terminal (qmake or cmake) 

# Clone the repository 
git clone https://github.com/abdelmesihchristine/TandemInsulinPumpSimulator.git 
cd pumpsimulator 

# QMake 
qmake TandemInsulinPumpSimulator.pro 
make 

# Run the executable 
./Tandem-Insulin-Pump-Simulator 


6. Usage Instructions: 

🔘 Main Interface: 
- See battery, insulin reservoir, CGM graph, time. 

💉 Bolus: 
- Click Bolus → Select Manual BG or CGM BG 
- Input carbs, BG, and IOB → Click Calculate then Deliver 

👤 Profiles: 
- Click Profiles → Add/Edit/Delete user configurations 
- Double-click to set an active profile for bolus logic 

📜 History: 
- Click History → Filter by bolus, CGM, or alerts 

🚨 Alerts: 
- Click Alerts → See unresolved or recent warnings 


7. System Design Principles 

📌 Observer Pattern: 
CGM simulator uses Qt signals to notify PumpController and CGMGraphWidget of new BG readings. 
Decouples components and enables event-driven updates. 

📌 Mediator Pattern: 
PumpController acts as a central coordinator between managers (Safety, History, User Profile, CGM). 
Reduces direct dependencies between components. 

📌 Encapsulation: 
Each manager class (e.g., BolusSafetyManager, UserProfileManager, HistoryManager) manages its own responsibilities. 
Improves modularity, readability, and maintainability. 

📌 Extensibility: 
Design allows easy addition of new features (e.g., exercise mode, sleep mode, new export formats). 
Supports future growth without major code rewrites. 
