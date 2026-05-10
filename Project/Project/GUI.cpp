#include "GUI.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Storage.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "PatientMenu.h"
#include "DoctorMenu.h"
#include "AdminMenu.h"
#include "FileHandling.h"
#include "Validator.h"
#include "HospitalException.h"
#include <iostream>
#include <sstream>

static const sf::Color COL_SAGE(125, 158, 126);
static const sf::Color COL_TEAL(61, 139, 139);
static const sf::Color COL_BEIGE(245, 240, 232);
static const sf::Color COL_DARKTEAL(42, 107, 107);
static const sf::Color COL_SHADOW(180, 200, 180);
static const sf::Color COL_WHITE(255, 255, 255);
static const sf::Color COL_DARK(40, 55, 40);
static const sf::Color COL_ERROR(200, 70, 70);
static const sf::Color COL_SUCCESS(61, 139, 139);

static const int WIN_W = 900;
static const int WIN_H = 640;

// ── helper
static std::string iToS(int n) {
    if (n == 0) return "0";
    std::string s = "";
    bool neg = n < 0;
    if (neg) n = -n;
    while (n > 0) { s = (char)('0' + n % 10) + s; n /= 10; }
    return neg ? std::string("-") + s : s;
}

static std::string fToS(float f) {
    int whole = (int)f;
    int dec = (int)((f - whole) * 100);
    if (dec < 0) dec = -dec;
    std::string d = iToS(dec);
    if (d.size() == 1) d = std::string("0") + d;
    return iToS(whole) + "." + d;
}

//button
Button::Button() { m_normalColor = COL_TEAL; m_hoverColor = COL_DARKTEAL; }

void Button::setup(sf::Vector2f pos, sf::Vector2f size,
    const std::string& label, sf::Font& font,
    sf::Color bgColor, sf::Color textColor,
    unsigned int fontSize) {
    m_pos = pos; m_size = size;
    m_normalColor = bgColor;
    m_hoverColor = sf::Color(
        (sf::Uint8)std::max(0, (int)bgColor.r - 35),
        (sf::Uint8)std::max(0, (int)bgColor.g - 35),
        (sf::Uint8)std::max(0, (int)bgColor.b - 35));
    m_shape.setSize(size);
    m_shape.setPosition(pos);
    m_shape.setFillColor(bgColor);
    m_text.setFont(font);
    m_text.setString(label);
    m_text.setCharacterSize(fontSize);
    m_text.setFillColor(textColor);
    centerText();
}

void Button::centerText() {
    sf::FloatRect tb = m_text.getLocalBounds();
    m_text.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    m_text.setPosition(m_pos.x + m_size.x / 2.f, m_pos.y + m_size.y / 2.f);
}

void Button::setLabel(const std::string& l) { m_text.setString(l); centerText(); }
void Button::setPosition(sf::Vector2f p) { m_pos = p; m_shape.setPosition(p); centerText(); }
sf::FloatRect Button::getBounds() { return m_shape.getGlobalBounds(); }

void Button::draw(sf::RenderWindow& w) {
    sf::Vector2i mouse = sf::Mouse::getPosition(w);
    m_shape.setFillColor(
        m_shape.getGlobalBounds().contains((float)mouse.x, (float)mouse.y)
        ? m_hoverColor : m_normalColor);
    w.draw(m_shape); w.draw(m_text);
}

bool Button::isHovered(sf::RenderWindow& w) {
    sf::Vector2i m = sf::Mouse::getPosition(w);
    return m_shape.getGlobalBounds().contains((float)m.x, (float)m.y);
}

bool Button::isClicked(sf::RenderWindow& w, sf::Event& e) {
    return e.type == sf::Event::MouseButtonReleased &&
        e.mouseButton.button == sf::Mouse::Left &&
        isHovered(w);
}

//asking for input
InputField::InputField() : m_isPassword(false) {}

void InputField::setup(sf::Vector2f pos, sf::Vector2f size,
    const std::string& label, sf::Font& font,
    bool isPassword) {
    m_pos = pos; m_size = size; m_isPassword = isPassword;
    m_box.setSize(size); m_box.setPosition(pos);
    m_box.setFillColor(COL_WHITE);
    m_box.setOutlineThickness(2);
    m_box.setOutlineColor(COL_SHADOW);
    m_label.setFont(font); m_label.setString(label);
    m_label.setCharacterSize(13);
    m_label.setFillColor(sf::Color(80, 100, 80));
    m_label.setPosition(pos.x, pos.y - 20);
    m_text.setFont(font); m_text.setCharacterSize(16);
    m_text.setFillColor(COL_DARK);
    m_text.setPosition(pos.x + 10, pos.y + m_size.y / 2.f - 10);
}

void InputField::draw(sf::RenderWindow& w, bool active) {
    m_box.setOutlineColor(active ? COL_TEAL : COL_SHADOW);
    w.draw(m_box); w.draw(m_label);
    std::string disp = m_isPassword ?
        std::string(m_value.size(), '*') : m_value;
    if (active) disp += "|";
    m_text.setString(disp);
    w.draw(m_text);
}

void InputField::handleEvent(sf::Event& e, bool active) {
    if (!active || e.type != sf::Event::TextEntered) return;
    char c = (char)e.text.unicode;
    if (c == '\b') { if (!m_value.empty()) m_value.pop_back(); }
    else if (c >= 32 && c < 127 && m_value.size() < 50) m_value += c;
}

std::string InputField::getValue() const { return m_value; }
void InputField::clear() { m_value = ""; }
bool InputField::containsPoint(float x, float y) {
    return m_box.getGlobalBounds().contains(x, y);
}


ScrollableList::ScrollableList() : m_font(nullptr),
m_scrollOffset(0), m_visibleLines(0) {
}

void ScrollableList::setup(sf::Vector2f pos, sf::Vector2f size,
    sf::Font& font) {
    m_pos = pos; m_size = size; m_font = &font;
    m_box.setSize(size); m_box.setPosition(pos);
    m_box.setFillColor(sf::Color(255, 255, 255, 200));
    m_box.setOutlineThickness(1);
    m_box.setOutlineColor(COL_SHADOW);
    m_visibleLines = (int)(size.y / 22);
}

void ScrollableList::clear() { m_lines.clear(); m_scrollOffset = 0; }

void ScrollableList::addLine(const std::string& line, sf::Color color) {
    m_lines.push_back({ line, color });
}

void ScrollableList::scroll(int delta) {
    m_scrollOffset += delta;
    int maxScroll = (int)m_lines.size() - m_visibleLines;
    if (m_scrollOffset < 0) m_scrollOffset = 0;
    if (m_scrollOffset > maxScroll && maxScroll > 0)
        m_scrollOffset = maxScroll;
}

void ScrollableList::draw(sf::RenderWindow& w) {
    w.draw(m_box);
    if (!m_font) return;
    int end = std::min((int)m_lines.size(),
        m_scrollOffset + m_visibleLines);
    for (int i = m_scrollOffset; i < end; i++) {
        sf::Text t;
        t.setFont(*m_font);
        t.setString(m_lines[i].first);
        t.setCharacterSize(14);
        t.setFillColor(m_lines[i].second);
        t.setPosition(m_pos.x + 8,
            m_pos.y + (i - m_scrollOffset) * 22 + 4);
        w.draw(t);
    }
    if ((int)m_lines.size() > m_visibleLines) {
        sf::Text hint;
        hint.setFont(*m_font);
        hint.setString("Scroll: UP/DOWN arrows");
        hint.setCharacterSize(11);
        hint.setFillColor(sf::Color(150, 160, 150));
        hint.setPosition(m_pos.x + 8, m_pos.y + m_size.y + 2);
        w.draw(hint);
    }
}

GUI::GUI()
    : m_window(sf::VideoMode(WIN_W, WIN_H),
        "MediCore HMS",
        sf::Style::Titlebar | sf::Style::Close),
    m_musicMuted(false), m_sfxMuted(false) {
    m_window.setFramerateLimit(60);
    loadAssets();
}

GUI::~GUI() { m_bgMusic.stop(); }

bool GUI::isOpen() const { return m_window.isOpen(); }

bool GUI::loadAssets() {
    if (!m_fontRegular.loadFromFile("assets/fonts/Poppins-Regular.ttf"))
        std::cout << "Warning: font not found\n";
    if (!m_fontBold.loadFromFile("assets/fonts/Poppins-Bold.ttf"))
        std::cout << "Warning: bold font not found\n";
    if (!m_clickBuffer.loadFromFile("assets/sounds/click.ogg"))
        std::cout << "Warning: click.ogg not found\n";
    else { m_clickSound.setBuffer(m_clickBuffer); m_clickSound.setVolume(60.f); }
    if (!m_bgMusic.openFromFile("assets/music/homepage.ogg"))
        std::cout << "Warning: music not found\n";
    else { m_bgMusic.setLoop(true); m_bgMusic.setVolume(40.f); }
    return true;
}

void GUI::playClick() { if (!m_sfxMuted) m_clickSound.play(); }

void GUI::drawBackground() {
    sf::RectangleShape bg({ (float)WIN_W,(float)WIN_H });
    bg.setFillColor(COL_BEIGE); m_window.draw(bg);
    sf::RectangleShape sb({ 200,(float)WIN_H });
    sb.setFillColor(COL_SAGE); m_window.draw(sb);
    sf::RectangleShape strip({ 6,(float)WIN_H });
    strip.setPosition(200, 0); strip.setFillColor(COL_TEAL);
    m_window.draw(strip);
    sf::RectangleShape tb({ (float)WIN_W,65 });
    tb.setFillColor(COL_DARKTEAL); m_window.draw(tb);
}

void GUI::drawHeader(const std::string& title) {
    sf::Text t;
    t.setFont(m_fontBold); t.setString(title);
    t.setCharacterSize(24); t.setFillColor(COL_BEIGE);
    t.setPosition(20, 17); m_window.draw(t);
}
//menu
int GUI::showMenuScreen(const std::string& title,
    const std::string& subtitle,
    const std::vector<std::string>& options) {

    const float startY = 80.f;   // first button top-Y
    const float bottomY = (float)WIN_H - 10.f; // last pixel we can use
    const float available = bottomY - startY;
    const int   n = (int)options.size();

    float btnH = 46.f;
    float gap = 10.f;

    float needed = n * btnH + (n - 1) * gap;
    if (needed > available) {
        float scale = available / needed;
        btnH *= scale;
        gap *= scale;
        if (btnH < 28.f) btnH = 28.f; 
    }

    float btnW = 580.f;
    float btnX = 230.f;

    std::vector<Button> btns((size_t)n);
    for (int i = 0; i < n; i++) {
        sf::Color c = (options[i] == "Logout" || options[i] == "Back")
            ? sf::Color(160, 90, 90)
            : COL_TEAL;
        
        unsigned int fs = (unsigned int)(btnH * 0.38f);
        if (fs < 11) fs = 11;
        if (fs > 17) fs = 17;

        btns[i].setup(
            { btnX, startY + i * (btnH + gap) },
            { btnW, btnH },
            options[i],
            m_fontRegular, c, COL_BEIGE, fs);
    }

    sf::Text sideText;
    sideText.setFont(m_fontBold);
    sideText.setString("MediCore");
    sideText.setCharacterSize(20);
    sideText.setFillColor(COL_BEIGE);
    sideText.setPosition(15, 80);

    sf::Text subText;
    subText.setFont(m_fontRegular);
    subText.setString(subtitle);
    subText.setCharacterSize(13);
    subText.setFillColor(COL_BEIGE);
    subText.setPosition(15, 108);

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close(); return -1;
            }
            for (int i = 0; i < n; i++) {
                if (btns[i].isClicked(m_window, event)) {
                    playClick();
                    return i;
                }
            }
        }
        m_window.clear();
        drawBackground();
        drawHeader(title);
        m_window.draw(sideText);
        m_window.draw(subText);
        for (auto& b : btns) b.draw(m_window);
        m_window.display();
    }
    return -1;
}

void GUI::showResultsScreen(const std::string& title,
    const std::vector<std::string>& lines) {
    ScrollableList list;
    list.setup({ 220, 80 }, { 660, 490 }, m_fontRegular);
    for (auto& l : lines) list.addLine(l);

    Button btnBack;
    btnBack.setup({ 340, 585 }, { 220, 40 },
        "Back", m_fontRegular,
        COL_SHADOW, COL_DARK, 15);

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close(); return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down) list.scroll(1);
                if (event.key.code == sf::Keyboard::Up)   list.scroll(-1);
            }
            if (event.type == sf::Event::MouseWheelScrolled)
                list.scroll(event.mouseWheelScroll.delta < 0 ? 1 : -1);
            if (btnBack.isClicked(m_window, event)) { playClick(); return; }
        }
        m_window.clear();
        drawBackground();
        drawHeader(title);
        list.draw(m_window);
        btnBack.draw(m_window);
        m_window.display();
    }
}

std::string GUI::promptInput(const std::string& question,
    bool isPassword) {
    InputField field;
    field.setup({ 230, 300 }, { 440, 48 },
        question, m_fontRegular, isPassword);
    field.clear();

    Button btnOK, btnCancel;
    btnOK.setup({ 280, 380 }, { 160, 44 },
        "OK", m_fontBold, COL_TEAL, COL_BEIGE, 18);
    btnCancel.setup({ 460, 380 }, { 160, 44 },
        "Cancel", m_fontRegular,
        COL_SHADOW, COL_DARK, 15);

    sf::Text qText;
    qText.setFont(m_fontBold);
    qText.setString(question);
    qText.setCharacterSize(16);
    qText.setFillColor(COL_DARKTEAL);
    qText.setPosition(230, 270);

    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close(); return "";
            }
            field.handleEvent(event, true);

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter) {
                playClick();
                return field.getValue();
            }
            if (btnOK.isClicked(m_window, event)) {
                playClick(); return field.getValue();
            }
            if (btnCancel.isClicked(m_window, event)) {
                playClick(); return "";
            }
        }
        m_window.clear();
        drawBackground();
        drawHeader("MediCore HMS");
        m_window.draw(qText);
        field.draw(m_window, true);
        btnOK.draw(m_window);
        btnCancel.draw(m_window);
        m_window.display();
    }
    return "";
}

int GUI::promptInt(const std::string& question) {
    std::string val = promptInput(question);
    if (val.empty()) return -1;
    int result = 0;
    for (char c : val)
        if (c >= '0' && c <= '9')
            result = result * 10 + (c - '0');
    return result;
}

float GUI::promptFloat(const std::string& question) {
    std::string val = promptInput(question);
    if (val.empty()) return 0.0f;
    float result = 0.0f;
    float dec = 0.1f;
    bool afterDot = false;
    for (char c : val) {
        if (c == '.') { afterDot = true; }
        else if (!afterDot) result = result * 10 + (c - '0');
        else { result += (c - '0') * dec; dec *= 0.1f; }
    }
    return result;
}
void GUI::showMessage(const std::string& msg) {
    Button btnOK;
    btnOK.setup({ 340,370 }, { 220,46 },
        "OK", m_fontBold, COL_TEAL, COL_BEIGE, 18);

    sf::Text t;
    t.setFont(m_fontBold); t.setString(msg);
    t.setCharacterSize(18); t.setFillColor(COL_DARKTEAL);
    t.setPosition(230, 280);

    while (m_window.isOpen()) {
        sf::Event e;
        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_window.close(); return;
            }
            if (btnOK.isClicked(m_window, e)) { playClick(); return; }
        }
        m_window.clear();
        drawBackground();
        drawHeader("MediCore HMS");
        m_window.draw(t);
        btnOK.draw(m_window);
        m_window.display();
    }
}

LoginResult GUI::runHomepage() {
    if (m_bgMusic.getDuration().asSeconds() > 0) m_bgMusic.play();

    sf::Text sideT; sideT.setFont(m_fontBold);
    sideT.setString("MediCore"); sideT.setCharacterSize(22);
    sideT.setFillColor(COL_BEIGE); sideT.setPosition(15, 80);

    sf::Text sideSub; sideSub.setFont(m_fontRegular);
    sideSub.setString("Hospital\nManagement\nSystem");
    sideSub.setCharacterSize(13); sideSub.setFillColor(COL_BEIGE);
    sideSub.setPosition(15, 108);

    sf::Text welcome; welcome.setFont(m_fontBold);
    welcome.setString("Welcome!"); welcome.setCharacterSize(28);
    welcome.setFillColor(COL_DARKTEAL); welcome.setPosition(230, 85);

    sf::Text sub; sub.setFont(m_fontRegular);
    sub.setString("Select your role to continue.");
    sub.setCharacterSize(14);
    sub.setFillColor(sf::Color(100, 120, 100));
    sub.setPosition(230, 122);

    sf::RectangleShape div({ 580,1 });
    div.setFillColor(COL_SHADOW); div.setPosition(220, 150);

    Button btnPat, btnDoc, btnAdm, btnQuit;
    btnPat.setup({ 230,165 }, { 560,55 }, "Patient Login",
        m_fontBold, COL_TEAL, COL_BEIGE, 17);
    btnDoc.setup({ 230,230 }, { 560,55 }, "Doctor Login",
        m_fontBold, COL_TEAL, COL_BEIGE, 17);
    btnAdm.setup({ 230,295 }, { 560,55 }, "Admin Login",
        m_fontBold, COL_SAGE, COL_BEIGE, 17);
    btnQuit.setup({ 230,375 }, { 560,45 }, "Exit",
        m_fontRegular, sf::Color(170, 90, 90), COL_BEIGE, 15);

    Button btnMusicToggle, btnSfxToggle;
    btnMusicToggle.setup({ 650,16 }, { 110,32 },
        "Music: ON", m_fontRegular, COL_TEAL, COL_BEIGE, 12);
    btnSfxToggle.setup({ 768,16 }, { 110,32 },
        "SFX: ON", m_fontRegular, COL_TEAL, COL_BEIGE, 12);

    sf::Text footer; footer.setFont(m_fontRegular);
    footer.setString("MediCore v1.0  |  OOP Semester Project 2026");
    footer.setCharacterSize(11);
    footer.setFillColor(sf::Color(150, 160, 150));
    footer.setPosition(230, 610);

    while (m_window.isOpen()) {
        sf::Event e;
        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_window.close(); return LoginResult::Quit;
            }
            if (btnPat.isClicked(m_window, e)) {
                playClick(); m_bgMusic.stop();
                return LoginResult::Patient;
            }
            if (btnDoc.isClicked(m_window, e)) {
                playClick(); m_bgMusic.stop();
                return LoginResult::Doctor;
            }
            if (btnAdm.isClicked(m_window, e)) {
                playClick(); m_bgMusic.stop();
                return LoginResult::Admin;
            }
            if (btnQuit.isClicked(m_window, e)) {
                playClick(); m_window.close();
                return LoginResult::Quit;
            }
            if (btnMusicToggle.isClicked(m_window, e)) {
                playClick(); m_musicMuted = !m_musicMuted;
                if (m_musicMuted) { m_bgMusic.pause(); btnMusicToggle.setLabel("Music: OFF"); }
                else { m_bgMusic.play();  btnMusicToggle.setLabel("Music: ON"); }
            }
            if (btnSfxToggle.isClicked(m_window, e)) {
                m_sfxMuted = false; playClick();
                m_sfxMuted = !m_sfxMuted;
                btnSfxToggle.setLabel(m_sfxMuted ? "SFX: OFF" : "SFX: ON");
            }
        }
        m_window.clear();
        drawBackground(); drawHeader("MediCore HMS");
        m_window.draw(sideT); m_window.draw(sideSub);
        m_window.draw(welcome); m_window.draw(sub);
        m_window.draw(div);
        btnPat.draw(m_window); btnDoc.draw(m_window);
        btnAdm.draw(m_window); btnQuit.draw(m_window);
        btnMusicToggle.draw(m_window); btnSfxToggle.draw(m_window);
        m_window.draw(footer);
        m_window.display();
    }
    return LoginResult::Quit;
}

bool GUI::runLoginScreen(const std::string& role,
    int& outID, std::string& outPwd,
    const std::string& errorMsg) {
    std::string idLabel = "Patient ID";
    if (role == "Doctor") idLabel = "Doctor ID";
    else if (role == "Admin")  idLabel = "Admin ID";

    InputField fID, fPwd;
    fID.setup({ 250,230 }, { 380,46 }, idLabel, m_fontRegular, false);
    fPwd.setup({ 250,320 }, { 380,46 }, "Password", m_fontRegular, true);
    fID.clear(); fPwd.clear();

    bool focusID = true;
    std::string localErr = errorMsg;

    Button btnLogin, btnBack;
    btnLogin.setup({ 300,400 }, { 280,50 },
        "Login", m_fontBold, COL_TEAL, COL_BEIGE, 19);
    btnBack.setup({ 300,462 }, { 280,38 },
        "Back", m_fontRegular, COL_SHADOW, COL_DARK, 14);

    sf::Text titleT; titleT.setFont(m_fontBold);
    titleT.setString(role + " Login");
    titleT.setCharacterSize(24); titleT.setFillColor(COL_DARKTEAL);
    titleT.setPosition(250, 100);

    sf::Text hintT; hintT.setFont(m_fontRegular);
    hintT.setString("Click field to select  |  Tab to switch  |  Enter to login");
    hintT.setCharacterSize(11);
    hintT.setFillColor(sf::Color(160, 170, 160));
    hintT.setPosition(250, 520);

    sf::Text errT; errT.setFont(m_fontRegular);
    errT.setCharacterSize(14); errT.setFillColor(COL_ERROR);
    errT.setPosition(250, 378);

    while (m_window.isOpen()) {
        sf::Event e;
        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_window.close(); return false;
            }
            if (e.type == sf::Event::MouseButtonReleased &&
                e.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)e.mouseButton.x;
                float my = (float)e.mouseButton.y;
                if (fID.containsPoint(mx, my))  focusID = true;
                if (fPwd.containsPoint(mx, my)) focusID = false;
            }
            if (e.type == sf::Event::KeyPressed &&
                e.key.code == sf::Keyboard::Tab)
                focusID = !focusID;

            if (focusID)  fID.handleEvent(e, true);
            else          fPwd.handleEvent(e, true);

            bool submit = false;
            if (e.type == sf::Event::KeyPressed &&
                e.key.code == sf::Keyboard::Enter) submit = true;
            if (btnLogin.isClicked(m_window, e)) submit = true;

            if (submit) {
                if (fID.getValue().empty() || fPwd.getValue().empty()) {
                    localErr = "Please fill in both fields.";
                }
                else {
                    outID = 0;
                    for (char c : fID.getValue())
                        if (c >= '0' && c <= '9') outID = outID * 10 + (c - '0');
                    outPwd = fPwd.getValue();
                    playClick(); return true;
                }
            }
            if (btnBack.isClicked(m_window, e)) { playClick(); return false; }
        }
        m_window.clear();
        drawBackground(); drawHeader("MediCore HMS");
        m_window.draw(titleT);
        fID.draw(m_window, focusID); fPwd.draw(m_window, !focusID);
        if (!localErr.empty()) { errT.setString(localErr); m_window.draw(errT); }
        btnLogin.draw(m_window); btnBack.draw(m_window);
        m_window.draw(hintT);
        m_window.display();
    }
    return false;
}

void GUI::runPatientMenuGUI(Patient& p,
    Storage<Doctor>& doctors,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions) {

    while (m_window.isOpen()) {
        std::string subtitle =
            std::string(p.getName()) +
            "  |  Balance: PKR " + fToS(p.getBalance());

        std::vector<std::string> opts = {
            "Book Appointment",
            "Cancel Appointment",
            "View My Appointments",
            "View My Medical Records",
            "View My Bills",
            "Pay Bill",
            "Top Up Balance",
            "Logout"
        };

        int choice = showMenuScreen("Patient Portal", subtitle, opts);
        if (choice < 0 || choice == 7) return; // logout or closed

        // ── Book Appointment ─────────────────────────────────
        if (choice == 0) {
           
            std::string spec = promptInput("Enter specialization (e.g. Cardiology):");
            if (spec.empty()) continue; // Cancel → back to patient menu

            std::vector<std::string> docLines;
            for (int i = 0; i < doctors.size(); i++) {
                Doctor& d = doctors.getALL()[i];
                if (Validator::compareInsensitive(d.getSpec(), spec.c_str())) {
                    docLines.push_back(
                        "ID:" + iToS(d.getID()) +
                        "  " + d.getName() +
                        "  Fee: PKR " + fToS(d.getFee()));
                }
            }
            if (docLines.empty()) {
                showMessage("No doctors found for: " + spec);
                continue;
            }
            showResultsScreen("Available Doctors", docLines);
            
            Doctor* doc = nullptr;
            while (m_window.isOpen()) {
                int docID = promptInt("Enter Doctor ID:");
                if (docID < 0) break; // Cancel
                doc = doctors.findbyID(docID);
                if (doc) break;
                showMessage("Doctor not found. Try again.");
            }
            if (!doc) continue;

            // Step 3: date (loop until valid or cancelled)
            // FIX 3: distinguish format error from value error
            std::string date;
            while (m_window.isOpen()) {
                date = promptInput("Enter date (YYYY-MM-DD):");
                if (date.empty()) break; // Cancel
                if (!Validator::isValidDateFormat(date.c_str())) {
                    showMessage("Invalid date format. Use YYYY-MM-DD (e.g. 2026-06-15).");
                }
                else if (!Validator::isValidDateValue(date.c_str())) {
                    showMessage("Date must be in year 2026 or later.");
                }
                else {
                    break; // valid
                }
            }
            if (date.empty()) continue;

            const char* allSlots[8] = {
                "09:00","10:00","11:00","12:00",
                "13:00","14:00","15:00","16:00"
            };
            std::vector<std::string> slotLines;
            for (int s = 0; s < 8; s++) {
                bool taken = false;
                for (int i = 0; i < appointments.size(); i++) {
                    Appointment& a = appointments.getALL()[i];
                    if (a.getdocID() == doc->getID()) {
                        bool sd = true;
                        for (int k = 0; k < 10; k++)
                            if (a.getDate()[k] != date[k]) { sd = false; break; }
                        bool ss = true;
                        for (int k = 0; k < 5; k++)
                            if (a.gettimeSlot()[k] != allSlots[s][k]) { ss = false; break; }
                        bool canc = Validator::compareInsensitive(a.getStatus(), "cancelled");
                        if (sd && ss && !canc) { taken = true; break; }
                    }
                }
                if (!taken) slotLines.push_back(allSlots[s]);
            }
            if (slotLines.empty()) {
                showMessage("No slots available on that date.");
                continue;
            }
            showResultsScreen("Available Slots on " + date, slotLines);

            std::string slot;
            while (m_window.isOpen()) {
                slot = promptInput("Enter time slot (e.g. 09:00):");
                if (slot.empty()) break; // Cancel
                if (!Validator::isValidTimeSlot(slot.c_str())) {
                    showMessage("Invalid time slot. Choose from: 09:00 - 16:00 (hourly).");
                    continue;
                }
                // check slot not taken
                bool taken = false;
                for (int i = 0; i < appointments.size(); i++) {
                    Appointment& a = appointments.getALL()[i];
                    if (a.getdocID() == doc->getID()) {
                        bool sd = true;
                        for (int k = 0; k < 10; k++)
                            if (a.getDate()[k] != date[k]) { sd = false; break; }
                        bool ss = true;
                        for (int k = 0; k < (int)slot.size(); k++)
                            if (a.gettimeSlot()[k] != slot[k]) { ss = false; break; }
                        bool canc = Validator::compareInsensitive(a.getStatus(), "cancelled");
                        if (sd && ss && !canc) { taken = true; break; }
                    }
                }
                if (taken) { showMessage("Slot already taken. Choose another."); }
                else { break; } // valid
            }
            if (slot.empty()) continue;

            // check balance
            if (p.getBalance() < doc->getFee()) {
                showMessage("Insufficient balance. Fee: PKR " + fToS(doc->getFee()));
                continue;
            }

            int newAppID = 1;
            for (int i = 0; i < appointments.size(); i++)
                if (appointments.getALL()[i].getID() >= newAppID)
                    newAppID = appointments.getALL()[i].getID() + 1;

            int newBillID = 1;
            for (int i = 0; i < bills.size(); i++)
                if (bills.getALL()[i].getID() >= newBillID)
                    newBillID = bills.getALL()[i].getID() + 1;

            char today[11]; Validator::getTodayDate(today);

            Appointment newApp(newAppID, p.getID(), doc->getID(),
                date.c_str(), slot.c_str(), "pending");
            appointments.add(newApp);
            FileHandler::appendAppointments(newApp);

            Bill newBill(newBillID, p.getID(), newAppID,
                doc->getFee(), "unpaid", today);
            bills.add(newBill);
            FileHandler::appendBills(newBill);

            p -= doc->getFee();
            FileHandler::updatePatient(p);

            showMessage("Booked! ID:" + iToS(newAppID) +
                "\nFee: PKR " + fToS(doc->getFee()) +
                "\nBalance: PKR " + fToS(p.getBalance()));
        }

        //cancle
        else if (choice == 1) {
            std::vector<std::string> lines;
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getALL()[i];
                if (a.getpatientID() == p.getID() &&
                    Validator::compareInsensitive(a.getStatus(), "pending")) {
                    Doctor* d = doctors.findbyID(a.getdocID());
                    lines.push_back("ID:" + iToS(a.getID()) +
                        "  Dr." + (d ? d->getName() : "?") +
                        "  " + a.getDate() + " " + a.gettimeSlot());
                }
            }
            if (lines.empty()) { showMessage("No pending appointments."); continue; }
            showResultsScreen("Pending Appointments", lines);

            // loop until valid ID or cancelled
            Appointment* target = nullptr;
            while (m_window.isOpen()) {
                int cancelID = promptInt("Enter Appointment ID to cancel:");
                if (cancelID < 0) break; // Cancel
                for (int i = 0; i < appointments.size(); i++) {
                    Appointment& a = appointments.getALL()[i];
                    if (a.getID() == cancelID &&
                        a.getpatientID() == p.getID() &&
                        Validator::compareInsensitive(a.getStatus(), "pending")) {
                        target = &a; break;
                    }
                }
                if (target) break;
                showMessage("Invalid appointment ID. Try again.");
            }
            if (!target) continue;

            Doctor* d = doctors.findbyID(target->getdocID());
            float refund = d ? d->getFee() : 0.0f;

            target->setStatus("cancelled");
            FileHandler::updateAppointment(*target);
            p += refund;
            FileHandler::updatePatient(p);

            for (int i = 0; i < bills.size(); i++) {
                Bill& b = bills.getALL()[i];
                if (b.getappID() == target->getID()) {
                    b.setstatus("cancelled");
                    FileHandler::updateBill(b); break;
                }
            }
            showMessage("Cancelled! Refund: PKR " + fToS(refund) +
                "\nBalance: PKR " + fToS(p.getBalance()));
        }
        else if (choice == 2) {
            std::vector<std::string> lines;
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getALL()[i];
                if (a.getpatientID() == p.getID()) {
                    Doctor* d = doctors.findbyID(a.getdocID());
                    lines.push_back(
                        "ID:" + iToS(a.getID()) +
                        "  Dr." + (d ? d->getName() : "?") +
                        "  " + a.getDate() + " " + a.gettimeSlot() +
                        "  [" + a.getStatus() + "]");
                }
            }
            if (lines.empty()) lines.push_back("No appointments found.");
            showResultsScreen("My Appointments", lines);
        }

        else if (choice == 3) {
            std::vector<std::string> lines;
            for (int i = 0; i < prescriptions.size(); i++) {
                Prescription& pr = prescriptions.getALL()[i];
                if (pr.getPatientID() == p.getID()) {
                    Doctor* d = doctors.findbyID(pr.getDoctorID());
                    lines.push_back(pr.getDate() +
                        std::string("  Dr.") + (d ? d->getName() : "?") +
                        "  " + pr.getMedicines());
                    lines.push_back("  Notes: " + std::string(pr.getNotes()));
                    lines.push_back("---");
                }
            }
            if (lines.empty()) lines.push_back("No records found.");
            showResultsScreen("Medical Records", lines);
        }

        else if (choice == 4) {
            std::vector<std::string> lines;
            double total = 0;
            for (int i = 0; i < bills.size(); i++) {
                Bill& b = bills.getALL()[i];
                if (b.getPatientID() == p.getID()) {
                    lines.push_back(
                        "Bill#" + iToS(b.getID()) +
                        "  PKR " + fToS(b.getamount()) +
                        "  [" + b.getStatus() + "]" +
                        "  " + b.getDate());
                    if (Validator::compareInsensitive(b.getStatus(), "unpaid"))
                        total += b.getamount();
                }
            }
            if (lines.empty()) lines.push_back("No bills found.");
            else lines.push_back("Total unpaid: PKR " + fToS((float)total));
            showResultsScreen("My Bills", lines);
        }
        else if (choice == 5) {
            std::vector<std::string> lines;
            for (int i = 0; i < bills.size(); i++) {
                Bill& b = bills.getALL()[i];
                if (b.getPatientID() == p.getID() &&
                    Validator::compareInsensitive(b.getStatus(), "unpaid")) {
                    lines.push_back("Bill#" + iToS(b.getID()) +
                        "  PKR " + fToS(b.getamount()));
                }
            }
            if (lines.empty()) { showMessage("No unpaid bills!"); continue; }
            showResultsScreen("Unpaid Bills", lines);

            // loop until valid bill ID or cancelled
            Bill* target = nullptr;
            while (m_window.isOpen()) {
                int billID = promptInt("Enter Bill ID to pay:");
                if (billID < 0) break; // Cancel
                for (int i = 0; i < bills.size(); i++) {
                    Bill& b = bills.getALL()[i];
                    if (b.getID() == billID &&
                        b.getPatientID() == p.getID() &&
                        Validator::compareInsensitive(b.getStatus(), "unpaid")) {
                        target = &b; break;
                    }
                }
                if (target) break;
                showMessage("Invalid bill ID. Try again.");
            }
            if (!target) continue;

            if (p.getBalance() < target->getamount()) {
                showMessage("Insufficient balance!"); continue;
            }
            p -= (float)target->getamount();
            FileHandler::updatePatient(p);
            target->setstatus("paid");
            FileHandler::updateBill(*target);
            showMessage("Paid! Balance: PKR " + fToS(p.getBalance()));
        }

        else if (choice == 6) {
            // loop until valid amount or cancelled
            while (m_window.isOpen()) {
                float amt = promptFloat("Enter amount to add (PKR):");
                if (amt == 0.0f) break; // Cancel (promptFloat returns 0 on empty)
                if (amt <= 0) { showMessage("Amount must be > 0."); continue; }
                p += amt;
                FileHandler::updatePatient(p);
                showMessage("Balance updated!\nNew balance: PKR " + fToS(p.getBalance()));
                break;
            }
        }
    }
}

void GUI::runDoctorMenuGUI(Doctor& d,
    Storage<Appointment>& appointments,
    Storage<Patient>& patients,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions) {

    while (m_window.isOpen()) {
        std::string subtitle =
            std::string(d.getName()) +
            "  |  " + d.getSpec();

        std::vector<std::string> opts = {
            "View Today's Appointments",
            "Mark Appointment Complete",
            "Mark Appointment No-Show",
            "Write Prescription",
            "View Patient Medical History",
            "Logout"
        };

        int choice = showMenuScreen("Doctor Portal", subtitle, opts);
        if (choice < 0 || choice == 5) return;

        if (choice == 0) {
            char today[11]; Validator::getTodayDate(today);
            std::vector<std::string> lines;
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getALL()[i];
                if (a.getdocID() != d.getID()) continue;
                bool sd = true;
                for (int k = 0; k < 10; k++)
                    if (a.getDate()[k] != today[k]) { sd = false; break; }
                if (!sd) continue;
                Patient* pat = patients.findbyID(a.getpatientID());
                lines.push_back(
                    "ID:" + iToS(a.getID()) +
                    "  " + (pat ? pat->getName() : "?") +
                    "  " + a.gettimeSlot() +
                    "  [" + a.getStatus() + "]");
            }
            if (lines.empty()) lines.push_back("No appointments today.");
            showResultsScreen("Today's Appointments", lines);
        }

        else if (choice == 1) {
            char today[11]; Validator::getTodayDate(today);
            std::vector<std::string> lines;
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getALL()[i];
                if (a.getdocID() != d.getID()) continue;
                bool sd = true;
                for (int k = 0; k < 10; k++)
                    if (a.getDate()[k] != today[k]) { sd = false; break; }
                if (!sd) continue;
                if (!Validator::compareInsensitive(a.getStatus(), "pending")) continue;
                Patient* pat = patients.findbyID(a.getpatientID());
                lines.push_back("ID:" + iToS(a.getID()) +
                    "  " + (pat ? pat->getName() : "?") +
                    "  " + a.gettimeSlot());
            }
            if (lines.empty()) { showMessage("No pending appointments today."); continue; }
            showResultsScreen("Pending Today", lines);

            // loop until valid ID or cancelled
            while (m_window.isOpen()) {
                int appID = promptInt("Enter Appointment ID:");
                if (appID < 0) break; // Cancel
                bool found = false;
                for (int i = 0; i < appointments.size(); i++) {
                    Appointment& a = appointments.getALL()[i];
                    bool sd = true;
                    for (int k = 0; k < 10; k++)
                        if (a.getDate()[k] != today[k]) { sd = false; break; }
                    if (a.getID() == appID && a.getdocID() == d.getID() &&
                        sd && Validator::compareInsensitive(a.getStatus(), "pending")) {
                        a.setStatus("completed");
                        FileHandler::updateAppointment(a);
                        showMessage("Marked as completed!");
                        found = true; break;
                    }
                }
                if (found) break;
                showMessage("Invalid appointment ID. Try again.");
            }
        }

        else if (choice == 2) {
            char today[11]; Validator::getTodayDate(today);
            std::vector<std::string> lines;
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getALL()[i];
                if (a.getdocID() != d.getID()) continue;
                bool sd = true;
                for (int k = 0; k < 10; k++)
                    if (a.getDate()[k] != today[k]) { sd = false; break; }
                if (!sd) continue;
                if (!Validator::compareInsensitive(a.getStatus(), "pending")) continue;
                lines.push_back("ID:" + iToS(a.getID()) +
                    "  " + a.gettimeSlot());
            }
            if (lines.empty()) { showMessage("No pending appointments today."); continue; }
            showResultsScreen("Pending Today", lines);

            while (m_window.isOpen()) {
                int appID = promptInt("Enter Appointment ID:");
                if (appID < 0) break;
                bool found = false;
                for (int i = 0; i < appointments.size(); i++) {
                    Appointment& a = appointments.getALL()[i];
                    char today2[11]; Validator::getTodayDate(today2);
                    bool sd = true;
                    for (int k = 0; k < 10; k++)
                        if (a.getDate()[k] != today2[k]) { sd = false; break; }
                    if (a.getID() == appID && a.getdocID() == d.getID() &&
                        sd && Validator::compareInsensitive(a.getStatus(), "pending")) {
                        a.setStatus("noshow");
                        FileHandler::updateAppointment(a);
                        for (int j = 0; j < bills.size(); j++) {
                            if (bills.getALL()[j].getappID() == appID) {
                                bills.getALL()[j].setstatus("cancelled");
                                FileHandler::updateBill(bills.getALL()[j]);
                                break;
                            }
                        }
                        showMessage("Marked as no-show.");
                        found = true; break;
                    }
                }
                if (found) break;
                showMessage("Invalid appointment ID. Try again.");
            }
        }

        else if (choice == 3) {
            while (m_window.isOpen()) {
                int appID = promptInt("Enter completed Appointment ID:");
                if (appID < 0) break;

                Appointment* target = nullptr;
                for (int i = 0; i < appointments.size(); i++) {
                    Appointment& a = appointments.getALL()[i];
                    if (a.getID() == appID && a.getdocID() == d.getID() &&
                        Validator::compareInsensitive(a.getStatus(), "completed")) {
                        target = &a; break;
                    }
                }
                if (!target) { showMessage("Invalid or not-completed appointment. Try again."); continue; }

                bool exists = false;
                for (int i = 0; i < prescriptions.size(); i++)
                    if (prescriptions.getALL()[i].getAppointmentID() == appID)
                    {
                        exists = true; break;
                    }
                if (exists) { showMessage("Prescription already written."); break; }

                std::string meds = promptInput("Medicines (e.g. Paracetamol 500mg):");
                if (meds.empty()) break;
                std::string notes = promptInput("Notes:");

                int newID = 1;
                for (int i = 0; i < prescriptions.size(); i++)
                    if (prescriptions.getALL()[i].getID() >= newID)
                        newID = prescriptions.getALL()[i].getID() + 1;

                char today[11]; Validator::getTodayDate(today);

                Prescription pr(newID, target->getpatientID(),
                    appID, d.getID(), today,
                    meds.c_str(), notes.c_str());
                prescriptions.add(pr);
                FileHandler::appendPrescriptions(pr);
                showMessage("Prescription saved!");
                break;
            }
        }

        else if (choice == 4) {
            // loop until valid patient ID or cancelled
            while (m_window.isOpen()) {
                int patID = promptInt("Enter Patient ID:");
                if (patID < 0) break;

                Patient* pat = patients.findbyID(patID);
                if (!pat) { showMessage("Patient not found. Try again."); continue; }

                bool hasCompleted = false;
                for (int i = 0; i < appointments.size(); i++) {
                    Appointment& a = appointments.getALL()[i];
                    if (a.getpatientID() == patID &&
                        a.getdocID() == d.getID() &&
                        Validator::compareInsensitive(a.getStatus(), "completed")) {
                        hasCompleted = true; break;
                    }
                }
                if (!hasCompleted) {
                    showMessage("Access denied - no completed appointments with this patient.");
                    break;
                }

                std::vector<std::string> lines;
                for (int i = 0; i < prescriptions.size(); i++) {
                    Prescription& pr = prescriptions.getALL()[i];
                    if (pr.getPatientID() == patID && pr.getDoctorID() == d.getID()) {
                        lines.push_back(std::string(pr.getDate()) +
                            "  " + pr.getMedicines());
                        lines.push_back("  Notes: " + std::string(pr.getNotes()));
                        lines.push_back("---");
                    }
                }
                if (lines.empty()) lines.push_back("No prescriptions found.");
                showResultsScreen("Patient History: " +
                    std::string(pat->getName()), lines);
                break;
            }
        }
    }
}
void GUI::runAdminMenuGUI(Admin* admin,
    Storage<Patient>& patients,
    Storage<Doctor>& doctors,
    Storage<Appointment>& appointments,
    Storage<Bill>& bills,
    Storage<Prescription>& prescriptions) {

    while (m_window.isOpen()) {
        std::vector<std::string> opts = {
            "Add Doctor",
            "Remove Doctor",
            "View All Patients",
            "View All Doctors",
            "View All Appointments",
            "View Unpaid Bills",
            "Discharge Patient",
            "View Security Log",
            "Generate Daily Report",
            "Logout"
        };

        int choice = showMenuScreen(
            "Admin Panel",
            std::string(admin->getName()),
            opts);
        if (choice < 0 || choice == 9) return;

        if (choice == 0) {
            // Add Doctor — validate each field, loop on failure
            std::string name = promptInput("Doctor name:");
            if (name.empty()) continue;

            std::string spec = promptInput("Specialization:");
            if (spec.empty()) continue;

            std::string cont;
            while (m_window.isOpen()) {
                cont = promptInput("Contact (11 digits):");
                if (cont.empty()) break;
                if (Validator::isValidContact(cont.c_str())) break;
                showMessage("Invalid contact. Must be exactly 11 digits.");
            }
            if (cont.empty()) continue;

            std::string pwd;
            while (m_window.isOpen()) {
                pwd = promptInput("Password (min 6 chars):", true);
                if (pwd.empty()) break;
                if (Validator::isValidPass(pwd.c_str())) break;
                showMessage("Password too short. Minimum 6 characters.");
            }
            if (pwd.empty()) continue;

            float fee = 0.f;
            while (m_window.isOpen()) {
                fee = promptFloat("Consultation fee (PKR):");
                if (fee == 0.f) break;
                if (Validator::isValidAmount(fee)) break;
                showMessage("Fee must be greater than 0.");
            }
            if (fee <= 0.f) continue;

            int newID = 1;
            for (int i = 0; i < doctors.size(); i++)
                if (doctors.getALL()[i].getID() >= newID)
                    newID = doctors.getALL()[i].getID() + 1;

            Doctor nd(newID, name.c_str(), spec.c_str(),
                cont.c_str(), pwd.c_str(), fee);
            doctors.add(nd);
            FileHandler::appendDoctors(nd);
            showMessage("Doctor added! ID: " + iToS(newID));
        }

        else if (choice == 1) {
            std::vector<std::string> lines;
            for (int i = 0; i < doctors.size(); i++) {
                Doctor& d = doctors.getALL()[i];
                lines.push_back("ID:" + iToS(d.getID()) +
                    "  " + d.getName() +
                    "  " + d.getSpec());
            }
            if (lines.empty()) { showMessage("No doctors."); continue; }
            showResultsScreen("All Doctors", lines);

            while (m_window.isOpen()) {
                int docID = promptInt("Enter Doctor ID to remove:");
                if (docID < 0) break;
                Doctor* t = doctors.findbyID(docID);
                if (!t) { showMessage("Doctor not found. Try again."); continue; }
                bool hasPending = false;
                for (int i = 0; i < appointments.size(); i++) {
                    Appointment& a = appointments.getALL()[i];
                    if (a.getdocID() == docID &&
                        Validator::compareInsensitive(a.getStatus(), "pending")) {
                        hasPending = true; break;
                    }
                }
                if (hasPending) { showMessage("Cannot remove - has pending appointments."); break; }
                doctors.removeID(docID);
                FileHandler::deleteDoctor(docID);
                showMessage("Doctor removed.");
                break;
            }
        }

        else if (choice == 2) {
            std::vector<std::string> lines;
            for (int i = 0; i < patients.size(); i++) {
                Patient& p = patients.getALL()[i];
                int unpaid = 0;
                for (int j = 0; j < bills.size(); j++)
                    if (bills.getALL()[j].getPatientID() == p.getID() &&
                        Validator::compareInsensitive(bills.getALL()[j].getStatus(), "unpaid"))
                        unpaid++;
                lines.push_back(
                    "ID:" + iToS(p.getID()) +
                    "  " + p.getName() +
                    "  Age:" + iToS(p.getAge()) +
                    "  Bal:PKR " + fToS(p.getBalance()) +
                    "  Unpaid bills:" + iToS(unpaid));
            }
            if (lines.empty()) lines.push_back("No patients.");
            showResultsScreen("All Patients", lines);
        }

        else if (choice == 3) {
            std::vector<std::string> lines;
            for (int i = 0; i < doctors.size(); i++) {
                Doctor& d = doctors.getALL()[i];
                lines.push_back(
                    "ID:" + iToS(d.getID()) +
                    "  " + d.getName() +
                    "  " + d.getSpec() +
                    "  Fee:PKR " + fToS(d.getFee()));
            }
            if (lines.empty()) lines.push_back("No doctors.");
            showResultsScreen("All Doctors", lines);
        }

        else if (choice == 4) {
            std::vector<std::string> lines;
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getALL()[i];
                Patient* pat = patients.findbyID(a.getpatientID());
                Doctor* doc = doctors.findbyID(a.getdocID());
                lines.push_back(
                    "ID:" + iToS(a.getID()) +
                    "  " + (pat ? pat->getName() : "?") +
                    "  Dr." + (doc ? doc->getName() : "?") +
                    "  " + a.getDate() + " " + a.gettimeSlot() +
                    "  [" + a.getStatus() + "]");
            }
            if (lines.empty()) lines.push_back("No appointments.");
            showResultsScreen("All Appointments", lines);
        }

        else if (choice == 5) {
            std::vector<std::string> lines;
            for (int i = 0; i < bills.size(); i++) {
                Bill& b = bills.getALL()[i];
                if (!Validator::compareInsensitive(b.getStatus(), "unpaid")) continue;
                Patient* pat = patients.findbyID(b.getPatientID());
                lines.push_back(
                    "Bill#" + iToS(b.getID()) +
                    "  " + (pat ? pat->getName() : "?") +
                    "  PKR " + fToS(b.getamount()) +
                    "  " + b.getDate());
            }
            if (lines.empty()) lines.push_back("No unpaid bills.");
            showResultsScreen("Unpaid Bills", lines);
        }

        else if (choice == 6) {
            std::vector<std::string> lines;
            for (int i = 0; i < patients.size(); i++) {
                Patient& p = patients.getALL()[i];
                lines.push_back("ID:" + iToS(p.getID()) + "  " + p.getName());
            }
            if (lines.empty()) { showMessage("No patients."); continue; }
            showResultsScreen("All Patients", lines);

            while (m_window.isOpen()) {
                int patID = promptInt("Enter Patient ID to discharge:");
                if (patID < 0) break;
                Patient* t = patients.findbyID(patID);
                if (!t) { showMessage("Patient not found. Try again."); continue; }

                bool hasUnpaid = false;
                for (int i = 0; i < bills.size(); i++)
                    if (bills.getALL()[i].getPatientID() == patID &&
                        Validator::compareInsensitive(bills.getALL()[i].getStatus(), "unpaid"))
                    {
                        hasUnpaid = true; break;
                    }
                if (hasUnpaid) { showMessage("Has unpaid bills. Cannot discharge."); break; }

                bool hasPending = false;
                for (int i = 0; i < appointments.size(); i++)
                    if (appointments.getALL()[i].getpatientID() == patID &&
                        Validator::compareInsensitive(appointments.getALL()[i].getStatus(), "pending"))
                    {
                        hasPending = true; break;
                    }
                if (hasPending) { showMessage("Has pending appointments. Cannot discharge."); break; }

                FileHandler::appendToDischarge(patID, patients,
                    appointments, bills, prescriptions);
                FileHandler::deletePatient(patID);
                FileHandler::deleteAppointmentsByPatient(patID);
                FileHandler::deleteBillsByPatient(patID);
                FileHandler::deletePrescriptionsByPatient(patID);
                patients.removeID(patID);
                for (int i = appointments.size() - 1; i >= 0; i--)
                    if (appointments.getALL()[i].getpatientID() == patID)
                        appointments.removeID(appointments.getALL()[i].getID());
                for (int i = bills.size() - 1; i >= 0; i--)
                    if (bills.getALL()[i].getPatientID() == patID)
                        bills.removeID(bills.getALL()[i].getID());
                for (int i = prescriptions.size() - 1; i >= 0; i--)
                    if (prescriptions.getALL()[i].getPatientID() == patID)
                        prescriptions.removeID(prescriptions.getALL()[i].getID());
                showMessage("Patient discharged successfully.");
                break;
            }
        }

        else if (choice == 7) {
            std::ifstream fin("security_log.txt");
            std::vector<std::string> lines;
            if (!fin) { lines.push_back("Log file not found."); }
            else {
                char line[300];
                while (fin.getline(line, 300))
                    if (line[0] != '\0') lines.push_back(line);
                if (lines.empty()) lines.push_back("No entries.");
            }
            showResultsScreen("Security Log", lines);
        }

        else if (choice == 8) {
            char today[11]; Validator::getTodayDate(today);
            std::vector<std::string> lines;
            lines.push_back("Date: " + std::string(today));
            lines.push_back("---");

            int pend = 0, comp = 0, canc = 0, ns = 0;
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getALL()[i];
                bool sd = true;
                for (int k = 0; k < 10; k++)
                    if (a.getDate()[k] != today[k]) { sd = false; break; }
                if (!sd) continue;
                if (Validator::compareInsensitive(a.getStatus(), "pending"))   pend++;
                else if (Validator::compareInsensitive(a.getStatus(), "completed")) comp++;
                else if (Validator::compareInsensitive(a.getStatus(), "cancelled")) canc++;
                else if (Validator::compareInsensitive(a.getStatus(), "noshow"))    ns++;
            }
            lines.push_back("Appointments Today:");
            lines.push_back("  Pending:" + iToS(pend) +
                "  Completed:" + iToS(comp) +
                "  Cancelled:" + iToS(canc) +
                "  No-Show:" + iToS(ns));

            double rev = 0;
            for (int i = 0; i < bills.size(); i++) {
                Bill& b = bills.getALL()[i];
                if (!Validator::compareInsensitive(b.getStatus(), "paid")) continue;
                bool sd = true;
                for (int k = 0; k < 10; k++)
                    if (b.getDate()[k] != today[k]) { sd = false; break; }
                if (sd) rev += b.getamount();
            }
            lines.push_back("Revenue Today: PKR " + fToS((float)rev));
            lines.push_back("---");
            lines.push_back("Outstanding Unpaid:");
            for (int i = 0; i < patients.size(); i++) {
                Patient& p = patients.getALL()[i];
                double owed = 0;
                for (int j = 0; j < bills.size(); j++)
                    if (bills.getALL()[j].getPatientID() == p.getID() &&
                        Validator::compareInsensitive(bills.getALL()[j].getStatus(), "unpaid"))
                        owed += bills.getALL()[j].getamount();
                if (owed > 0) lines.push_back(
                    "  " + std::string(p.getName()) +
                    " owes PKR " + fToS((float)owed));
            }
            showResultsScreen("Daily Report", lines);
        }
    }
}
