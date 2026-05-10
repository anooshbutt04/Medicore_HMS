#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

// forward declarations
class Patient;
class Doctor;
class Admin;
template<typename T> class Storage;
class Appointment;
class Bill;
class Prescription;

enum class LoginResult { None, Patient, Doctor, Admin, Quit };


class Button {
public:
    Button();
    void setup(sf::Vector2f pos, sf::Vector2f size,
        const std::string& label, sf::Font& font,
        sf::Color bgColor, sf::Color textColor,
        unsigned int fontSize = 18);
    void draw(sf::RenderWindow& window);
    bool isHovered(sf::RenderWindow& window);
    bool isClicked(sf::RenderWindow& window, sf::Event& event);
    void setLabel(const std::string& label);
    void setPosition(sf::Vector2f pos);
    sf::FloatRect getBounds();
private:
    sf::RectangleShape m_shape;
    sf::Text           m_text;
    sf::Color          m_normalColor;
    sf::Color          m_hoverColor;
    sf::Vector2f       m_size;
    sf::Vector2f       m_pos;
    void centerText();
};


class InputField {
public:
    InputField();
    void setup(sf::Vector2f pos, sf::Vector2f size,
        const std::string& label, sf::Font& font,
        bool isPassword = false);
    void draw(sf::RenderWindow& window, bool active);
    void handleEvent(sf::Event& event, bool active);
    std::string getValue() const;
    void clear();
    bool containsPoint(float x, float y);
private:
    sf::RectangleShape m_box;
    sf::Text           m_label;
    sf::Text           m_text;
    std::string        m_value;
    bool               m_isPassword;
    sf::Vector2f       m_pos;
    sf::Vector2f       m_size;
};


class ScrollableList {
public:
    ScrollableList();
    void setup(sf::Vector2f pos, sf::Vector2f size, sf::Font& font);
    void clear();
    void addLine(const std::string& line,
        sf::Color color = sf::Color(40, 55, 40));
    void draw(sf::RenderWindow& window);
    void scroll(int delta); // +1 = down, -1 = up
private:
    sf::RectangleShape m_box;
    sf::Font* m_font;
    sf::Vector2f       m_pos;
    sf::Vector2f       m_size;
    std::vector<std::pair<std::string, sf::Color>> m_lines;
    int                m_scrollOffset;
    int                m_visibleLines;
};


class GUI {
public:
    GUI();
    ~GUI();

    // homepage
    LoginResult runHomepage();

    // login screen — returns true if credentials entered
    bool runLoginScreen(const std::string& role,
        int& outID, std::string& outPwd,
        const std::string& errorMsg = "");

    // role menus — fully in SFML
    void runPatientMenuGUI(Patient& p,
        Storage<Doctor>& doctors,
        Storage<Appointment>& appointments,
        Storage<Bill>& bills,
        Storage<Prescription>& prescriptions);

    void runDoctorMenuGUI(Doctor& d,
        Storage<Appointment>& appointments,
        Storage<Patient>& patients,
        Storage<Bill>& bills,
        Storage<Prescription>& prescriptions);

    void runAdminMenuGUI(Admin* admin,
        Storage<Patient>& patients,
        Storage<Doctor>& doctors,
        Storage<Appointment>& appointments,
        Storage<Bill>& bills,
        Storage<Prescription>& prescriptions);

    // popup message
    void showMessage(const std::string& msg);

    // input popup — asks user to type something
    // returns what they typed, empty if cancelled
    std::string promptInput(const std::string& question,
        bool isPassword = false);
    int promptInt(const std::string& question);
    float promptFloat(const std::string& question);

    bool isOpen() const;

private:
    sf::RenderWindow m_window;
    sf::Font         m_fontRegular;
    sf::Font         m_fontBold;
    sf::Music        m_bgMusic;
    sf::SoundBuffer  m_clickBuffer;
    sf::Sound        m_clickSound;
    bool             m_musicMuted;
    bool             m_sfxMuted;

    void playClick();
    void drawBackground();
    void drawHeader(const std::string& title);
    bool loadAssets();

    // builds a menu screen with buttons, returns chosen index (0-based)
    // or -1 if window closed
    int showMenuScreen(const std::string& title,
        const std::string& subtitle,
        const std::vector<std::string>& options);

    // shows a scrollable results screen
    void showResultsScreen(const std::string& title,
        const std::vector<std::string>& lines);
};

#endif