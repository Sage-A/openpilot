
class StatusWidget : public QWidget {
   Q_OBJECT
 public:
   explicit StatusWidget(QWidget *parent);
   void update(const SubMaster &sm);
 
 private:
   QLabel *fuel_value;
   QLabel *door_value;
   QLabel *seatbelt_value;
   QLabel *gas_value;
   QLabel *brake_value;
   QLabel *ss_value;
   QLabel *steer_enabled;
   QLabel *steer_value;
   QLabel *esp_value;
   QLabel *cruise_value;
   QLabel *gear_value;
 };
