#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

string getDeptName(Department d){
    switch(d){
        case CARDIOLOGY: return "Cardiology";
        case NEUROLOGY: return "Neurology";
        case ORTHOPEDICS: return "Orthopedics";
        case PEDIATRICS: return "Pediatrics";
        case EMERGENCY: return "Emergency";
        case GENERAL: return "General";
    }
    return "";
}

// ========== PATIENT CLASS ========== //
class Patient {
private:
    int id;
    string name;
    int age;
    string contact;
    stack<string> medicalHistory;
    queue<string> testQueue;
    bool isAdmitted;
    RoomType roomType;
    
public:
    Patient(int pid, string n, int a, string c) {
        id = pid;
        name = n;
        age = a;
        contact = c;
        isAdmitted = false;
    }
   

    void admitPatient(RoomType type) {
        if (isAdmitted) {
            cout << "Patient already admitted." << endl;
            return;
        }
        isAdmitted = true;
        this->roomType = type;
    }

    void dischargePatient() {
        if (!isAdmitted) {
            cout << "Patient not admitted." << endl;
            return;
        }
        isAdmitted = false;
        cout << "Patient " << name << " discharged." << endl;
    }

    void addMedicalRecord(string record) {
        medicalHistory.push(record);
    }
    void requestTest(string test) {
        testQueue.push(test);
    }

    string performTest()
    {
        if (testQueue.empty()) {
            return "No tests pending.";
        }

        string test = testQueue.front();
        testQueue.pop();
        return "Performing test: " + test;
    };

    void displayHistory() {
        if (medicalHistory.empty()) {
            cout << "No history." << endl;
            return;
        }

        stack<string> temp = medicalHistory;
        cout << "History (Newest → Oldest): " << endl;
        while (!temp.empty()) {
            cout << "- " << temp.top() << endl;
            temp.pop();
        }
    }

    
    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    bool getAdmissionStatus() {
        return isAdmitted;
    }
    int getAge(){
        return this->age;
    } // Added for functions like displayPatientInfo()
    RoomType getRoomType(){
        return this->roomType;
    }
};

// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;
    queue<int> appointmentQueue;
    
public:
    Doctor(int did, string n, Department d){
        this->id = did;
        this->name = n;
        this->department = d;
    }
    
    void addAppointment(int patientId){
        appointmentQueue.emplace(patientId);
        cout << "Appointment added for patient ID: " << patientId << " with Dr. " << this->name << endl;
    }
    int seePatient(){
        if(appointmentQueue.empty()){
            cout << "No appointments for Dr. " << this->name << endl;
            return -1;
        }
        int patientId = appointmentQueue.front();
        appointmentQueue.pop();
        return patientId;
    }
    
    int getId(){
        return this->id;
    }
    string getName(){
        return this->name;
    }
    string getDepartment(){
        return getDeptName(this->department);
    }
    int getPendingAppointments(){
        return appointmentQueue.size();
    }
};

// ========== HOSPITAL CLASS ========== //
class Hospital {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    queue<int> emergencyQueue;
    int patientCounter;
    int doctorCounter;
    
public:
    Hospital(){
        patientCounter = 0;
        doctorCounter = 0;
    };
    
    int registerPatient(string name, int age, string contact){
        Patient p(patientCounter, name , age , contact);
        patients.push_back(p);
        cout << "Patient registered: " << name << " (ID: " << patientCounter << ")" << endl;     
        
        return patientCounter++;
    }
    int addDoctor(string name, Department dept){
        Doctor d(doctorCounter,name,dept);
        doctors.push_back(d);
        cout << "Doctor added: " << name << " (ID: " << doctorCounter << ") - " << getDeptName(dept) << endl;
        return doctorCounter++;
    }
    void admitPatient(int patientId, RoomType type){
        for(auto &p : patients){
            if(p.getAdmissionStatus() && p.getId()==patientId){
                cout << "Patients is already admitted into a room" << endl;
                return;
            }else if(!p.getAdmissionStatus() && p.getId()==patientId){
                p.admitPatient(type);
                switch (type)
                {
                case RoomType::GENERAL_WARD:
                    cout << "Patient has been admitted into a General ward room." << endl;
                    break;
                case RoomType::ICU:
                    cout << "Patient has been admitted into a Intensive Care Unit room." << endl;
                    break;
                case RoomType::PRIVATE_ROOM:
                    cout << "Patient has been admitted into a private room." << endl;
                    break;
                case RoomType::SEMI_PRIVATE:
                    cout << "Patient has been admitted into a semi-private room." << endl;
                    break;
                }
                return;
            }
        }
        cout << "Patient not found." << endl;
    }
    void addEmergency(int patientId){
        for(auto &p : patients){
            if(p.getId()==patientId){
                emergencyQueue.push(patientId);
                cout << "Emergency added for patient: " << p.getName() << endl;
                return;
            }
        }
        cout << "Patient not found." << endl;
    }
    int handleEmergency(){
        if (emergencyQueue.empty()) {
            cout << "No emergency cases." << endl;
            return -1;
        }
        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handling emergency for patient: " << patientId << endl;
        return patientId;
    }
    void bookAppointment(int doctorId, int patientId){
        bool found=false;
        int doctorIndex=-1;
        for(auto &d : doctors){
            doctorIndex++;
            if(d.getId()==doctorId){
                found=true;
                break;
            }
        }
        if(!found){
            cout << "Doctor not found." << endl;
            return;
        }
        found=false;
        for(auto &p : patients){
            if(p.getId()==patientId){
                found=true;
                break;
            }
        }
        if(!found){
            cout << "Patient not found." << endl;
            return;
        }
        doctors[doctorIndex].addAppointment(patientId);
        cout << "Appointment has been booked with Dr." << doctors[doctorIndex].getName() << endl;
    }
    void displayPatientInfo(int patientId){
        for(auto &p : patients){
            if(p.getId()==patientId){
                cout << "=== Patient Information ===" << endl;
                cout << "ID: " << patientId << endl; 
                cout << "Name: " << p.getName() << endl;
                cout << "Age: " << p.getAge() << endl;
                if(p.getAdmissionStatus()){
                    cout << "Admission Status: Admitted" << endl;
                }else{
                    cout << "Admission Status: Not Admitted" << endl;
                }
                if(p.getAdmissionStatus()){
                    cout << "Room Type: " << p.getRoomType() << endl;
                }
                return;
            }
        }
        cout << "Patient not found." << endl;
    }
    void displayDoctorInfo(int doctorId){
        for(auto &d : doctors){
            if(d.getId()==doctorId){
                cout << "=== Doctor Information ===" << endl;
                cout << "ID: " << doctorId << endl; 
                cout << "Name: " << d.getName() << endl;
                cout << "Department: " << d.getDepartment() << endl;
                cout << "Pending Appointments: " << d.getPendingAppointments() << endl;
                return;
            }
        }
        cout << "Doctor not found." << endl;
}   // For test case 5 (BEGIN)
    void requestTestForPatient(int patientId, string test){
        for(auto &p : patients){
            if(p.getId()==patientId){
                p.requestTest(test);
                cout << "Test requested: " << test << " for patient: " << p.getName() << endl;
                return;
            }
        }
        cout << "Patient not found." << endl;
    }
    void performTestForPatient(int patientId){
        for(auto &p : patients){
            if(p.getId()==patientId){
                string result = p.performTest();
                cout << result << endl;
                return;
            }
        }
        cout << "Patient not found." << endl;
    } 
    // (END)
    // For test case 7 (BEGIN)
    void dischargePatient(int patientId){
        for(auto &p : patients){
            if(p.getId()==patientId){
                p.dischargePatient();
                return;
            }
        }
        cout << "Patient not found." << endl;
    }
    // (END)
    // For test case 9 (BEGIN)
    void doctorSeePatient(int doctorId){
        for(auto &d : doctors){
            if(d.getId()==doctorId){
                int patientId = d.seePatient();
                if(patientId != -1){
                    cout << "Dr. " << d.getName() << " is now seeing patient ID: " << patientId << endl;
                }
                return;
            }
        }
        cout << "Doctor not found." << endl;
    }
    // (END)
};

// ========== MAIN PROGRAM ========== //
int main() {
    Hospital hospital;
    
    // Test Case 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");
    
    // Test Case 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);
    
    // Test Case 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    // Try admitting already admitted patient
    hospital.admitPatient(p1, SEMI_PRIVATE);
    
    // Test Case 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    // Try booking with invalid doctor/patient
    hospital.bookAppointment(999, p1); // Invalid doctor
    hospital.bookAppointment(d1, 999); // Invalid patient
    
    // Test Case 5: Handling medical tests
    // These would normally be called on Patient objects
    // In a real implementation, we'd need a way to access patients
    hospital.requestTestForPatient(p1, "Blood Test");
    hospital.requestTestForPatient(p1, "X-Ray");
    hospital.performTestForPatient(p1); // Performs Blood Test
    hospital.performTestForPatient(p1); // Performs X-Ray
    // Test Case 6: Emergency cases
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    int emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency();
    emergencyPatient = hospital.handleEmergency(); // No more emergencies
    
    // Test Case 7: Discharging patients
    // Would normally call dischargePatient() on Patient objects
    hospital.dischargePatient(p1);
    hospital.dischargePatient(p2);
    // Test Case 8: Displaying information
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999); // Invalid patient
    
    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999); // Invalid doctor
    
    // Test Case 9: Doctor seeing patients
    // These would normally be called on Doctor objects
    // In a real implementation, we'd need a way to access doctors
    hospital.doctorSeePatient(d1); // Dr. Smith sees patient p1
    hospital.doctorSeePatient(d1); // Dr. Smith sees patient p2
    hospital.doctorSeePatient(d1); // No more patients for Dr. Smith
    hospital.doctorSeePatient(999); // Invalid doctor
    // Test Case 10: Edge cases
    Hospital emptyHospital;
    emptyHospital.displayPatientInfo(1); // No patients
    emptyHospital.displayDoctorInfo(1);  // No doctors
    emptyHospital.handleEmergency();     // No emergencies
    
    return 0;
}
