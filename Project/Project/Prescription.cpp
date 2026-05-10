#include "Prescription.h"

Prescription::Prescription() {
	presID = 0;
    patientID = 0;
	appID = 0;
	docID = 0;
	date[0] = '\0';
	medicines[0] = '\0';
	notes[0]='\0';
}

Prescription::Prescription(int presID, int patientID, int appID,
	int docID, const char* date, const char* medicines, const char* notes) {

	this->presID = presID;
	this->patientID = patientID;
	this->appID = appID;
	this->docID = docID;

    int i = 0;
    while (date[i] != '\0' && i < 10) {
        this->date[i] = date[i];
        i++;
    }
    this->date[i] = '\0';

   
    int j = 0;
    while (medicines[j] != '\0' && j < 499) {
        this->medicines[j] = medicines[j];
        j++;
    }
    this->medicines[j] = '\0';

    int k = 0;
    while (notes[k] != '\0' && k < 299) {
        this->notes[k] = notes[k];
        k++;
    }
    this->notes[k] = '\0';
}

Prescription::~Prescription() {}

void Prescription::setMedicines(const char* m) {
    int i = 0;
    while (m[i] != '\0' && i < 499) {
        medicines[i] = m[i];
        i++;
    }
    medicines[i] = '\0';
}

void Prescription::setNotes(const char* n) {
    int i = 0;
    while (n[i] != '\0' && i < 299) {
        notes[i] = n[i];
        i++;
    }
    notes[i] = '\0';
}

ostream& operator<<(ostream& out, const Prescription& p) {
    out << "Prescription ID: " << p.presID
        << " | Patient ID: " << p.patientID
        << " | Appointment ID: " << p.appID
        << " | Doctor ID: " << p.docID
        << " | Date: " << p.date
        << " | Medicines: " << p.medicines
        << " | Notes: " << p.notes;
    return out;
}


