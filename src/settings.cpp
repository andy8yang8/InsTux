#include "settings.h"

bool Settings::Aimbot::enable = true;
ButtonCode_t Settings::Aimbot::key = ButtonCode_t::KEY_LSHIFT;
float Settings::Aimbot::sensitivity = 1;
float Settings::Aimbot::ku = 7;
float Settings::Aimbot::tu = 0.23;
float Settings::Aimbot::fov = 10.0;

bool Settings::ESP::enable = true;
ButtonCode_t Settings::ESP::key = KEY_CAPSLOCK;
bool Settings::ESP::show_bone = true;
bool Settings::ESP::show_name = true;

bool Settings::Radar::enable = true;
int Settings::Radar::size = 100;
float Settings::Radar::range = 2000;
float Settings::Radar::dot_radius = 4;
Color Settings::Radar::color_friendly = Color(50, 255, 50, 180);
Color Settings::Radar::color_hostile = Color(255, 108, 10, 250);

void GetVal(Json::Value &config, int* setting)
{
    if (config.isNull())
        return;

    *setting = config.asInt();
}

void GetVal(Json::Value &config, bool* setting)
{
    if (config.isNull())
        return;

    *setting = config.asBool();
}

void GetVal(Json::Value &config, float* setting)
{
    if (config.isNull())
        return;

    *setting = config.asFloat();
}

void GetVal(Json::Value &config, Color* setting)
{
    if (config.isNull())
        return;

    GetVal(config["r"], &setting->r);
    GetVal(config["g"], &setting->g);
    GetVal(config["b"], &setting->b);
    GetVal(config["a"], &setting->a);
}

/*
void GetVal(Json::Value &config, ImColor* setting)
{
    if (config.isNull())
        return;

    GetVal(config["r"], &setting->Value.x);
    GetVal(config["g"], &setting->Value.y);
    GetVal(config["b"], &setting->Value.z);
    GetVal(config["a"], &setting->Value.w);
}
*/

void GetVal(Json::Value &config, char** setting)
{
    if (config.isNull())
        return;

    *setting = strdup(config.asCString());
}

void GetVal(Json::Value &config, char* setting)
{
    if (config.isNull())
        return;

    strcpy(setting, config.asCString());
}

template <typename Ord, Ord (*lookupFunction)(std::string)>
void GetOrdinal(Json::Value& config, Ord* setting)
{
    if (config.isNull())
        return;

    Ord value;
    if (config.isString())
        value = lookupFunction(config.asString());
    else
        value = (Ord) config.asInt();

    *setting = value;
}

void GetButtonCode(Json::Value &config, enum ButtonCode_t* setting)
{
    GetOrdinal<enum ButtonCode_t, Util::GetButtonCode>(config, setting);
}

void LoadColor(Json::Value &config, Color color)
{
    config["r"] = color.r;
    config["g"] = color.g;
    config["b"] = color.b;
    config["a"] = color.a;
}

void Settings::LoadDefaultsOrSave(std::string path)
{
    Json::Value settings;

    // inflate settings with default values
    settings["Aimbot"]["enable"] = Settings::Aimbot::enable;
    settings["Aimbot"]["key"] = Util::GetButtonName(Settings::Aimbot::key);
    settings["Aimbot"]["sensitivity"] = Settings::Aimbot::sensitivity;
    settings["Aimbot"]["fov"] = Settings::Aimbot::fov;
    settings["Aimbot"]["tu"] = Settings::Aimbot::tu;
    settings["Aimbot"]["ku"] = Settings::Aimbot::ku;

    settings["ESP"]["enable"] = Settings::ESP::enable;
    settings["ESP"]["key"] = Util::GetButtonName(Settings::ESP::key);
    settings["ESP"]["show_bone"] = Settings::ESP::show_bone;
    settings["ESP"]["show_name"] = Settings::ESP::show_name;

    settings["Radar"]["enable"] = Settings::Radar::enable;
    settings["Radar"]["size"] = Settings::Radar::size;
    settings["Radar"]["range"] = Settings::Radar::range;
    settings["Radar"]["dot_radius"] = Settings::Radar::dot_radius;
    LoadColor(settings["Radar"]["color_friendly"], Settings::Radar::color_friendly);
    LoadColor(settings["Radar"]["color_hostile"], Settings::Radar::color_hostile);
    
    Json::StyledWriter styledWriter;
    std::ofstream(path) << styledWriter.write(settings);
}

void Settings::LoadConfig()
{
    pstring path = getenv("HOME");
    path << "/.instux.cfg";

    if (!std::ifstream(path).good())
    {
        Settings::LoadDefaultsOrSave(path);
        return;
    }

    Json::Value settings;
    std::ifstream configDoc(path, std::ifstream::binary);
    configDoc >> settings;

    GetVal(settings["Aimbot"]["enable"], &Settings::Aimbot::enable);
    GetButtonCode(settings["Aimbot"]["key"], &Settings::Aimbot::key);
    GetVal(settings["Aimbot"]["sensitivity"], &Settings::Aimbot::sensitivity);
    GetVal(settings["Aimbot"]["fov"], &Settings::Aimbot::fov);
    GetVal(settings["Aimbot"]["tu"], &Settings::Aimbot::tu);
    GetVal(settings["Aimbot"]["ku"], &Settings::Aimbot::ku);

    GetVal(settings["ESP"]["enable"], &Settings::ESP::enable);
    GetButtonCode(settings["ESP"]["key"], &Settings::ESP::key);
    GetVal(settings["ESP"]["show_bone"], &Settings::ESP::show_bone);
    GetVal(settings["ESP"]["show_name"], &Settings::ESP::show_name);

    GetVal(settings["Radar"]["enable"], &Settings::Radar::enable);
    GetVal(settings["Radar"]["size"], &Settings::Radar::size);
    GetVal(settings["Radar"]["range"], &Settings::Radar::range);
    GetVal(settings["Radar"]["dot_radius"], &Settings::Radar::dot_radius);
    GetVal(settings["Radar"]["color_friendly"], &Settings::Radar::color_friendly);
    GetVal(settings["Radar"]["color_hostile"], &Settings::Radar::color_hostile);
}
