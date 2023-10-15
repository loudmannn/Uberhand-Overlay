#include <tesla.hpp>
#include <utils.hpp>

class JsonInfoOverlay : public tsl::Gui {
private:
    std::string jsonPath, specficKey;
    std::string kipPath = "/atmosphere/kips/loader.kip";

public:
    JsonInfoOverlay(std::string jsonPath, std::string specficKey) : jsonPath(jsonPath), specficKey(specficKey) {}
    ~JsonInfoOverlay() {}

    virtual tsl::elm::Element* createUI() override {
        // logMessage ("JsonInfoOverlay");

        std::pair<std::string, int> textDataPair;
        constexpr int lineHeight = 20;  // Adjust the line height as needed
        constexpr int fontSize = 19;    // Adjust the font size as needed

        auto rootFrame = new tsl::elm::OverlayFrame(specficKey, "Uberhand Package","",false,"\uE0E1  Back     \uE0E0  Apply     ");
        auto list = new tsl::elm::List();

        if (!isFileOrDirectory(jsonPath)) {
            list->addItem(new tsl::elm::CustomDrawer([lineHeight, fontSize](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h) {
            renderer->drawString("JSON file not found.\nContact the package dev.", false, x, y + lineHeight, fontSize, a(tsl::style::color::ColorText));
            }), fontSize + lineHeight);
            rootFrame->setContent(list);
            return rootFrame;
        } else {
            textDataPair = dispRAMTmpl(jsonPath, specficKey);
            std::string textdata = textDataPair.first;
            int textsize = textDataPair.second;
            if (!textdata.empty()) {
                list->addItem(new tsl::elm::CustomDrawer([lineHeight, fontSize, textdata](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h) {
                renderer->drawString(textdata.c_str(), false, x, y + lineHeight, fontSize, a(tsl::style::color::ColorText));
                }), fontSize * textsize + lineHeight);
                rootFrame->setContent(list);
            }
        }
        return rootFrame;
    }

    std::map <std::string,std::string> parseJson (std::string jsonPath, std::string selectedItem) {
        std::map <std::string,std::string> newKipdata;
        std::vector<std::string> offsets = {"32","48","16","36","52","56","60","64","68","76"};
        std::vector<std::string> offsetStrs = findHexDataOffsets(kipPath, "43555354"); // 43555354 is a CUST
        if (!offsetStrs.empty()) {
            for(auto& offset : offsets) {
                offset = std::to_string(std::stoi(offset) + std::stoi(offsetStrs[0])); // count from "C" letter
            }
        }

        json_t* jsonData = readJsonFromFile(jsonPath);
        if (jsonData) {
            size_t arraySize = json_array_size(jsonData);

            for (size_t i = 0; i < arraySize; ++i) {
                json_t* item = json_array_get(jsonData, i);
                json_t* keyValue = json_object_get(item, "name");

                if (json_object_size(item) != offsets.size() + 1) { return newKipdata; }

                if (json_string_value(keyValue) == selectedItem) {
                    const char *key;
                    json_t *value;
                    int j = 0;
                    json_object_foreach(item, key, value) {
                        if (strcmp(key, "name") != 0) {
                            std::string valStr = json_string_value(value);
                            size_t spacePos = valStr.find(' ');
                            if (spacePos != 0) { valStr = valStr.substr(0, spacePos); }
                            if (valStr.length() ==4) { valStr = std::to_string(std::stoi(valStr) * 1000); }
                            else if (valStr.length() ==3) { valStr = std::to_string(std::stoi(valStr) * 1000); }
                            newKipdata.emplace(offsets[j], decimalToReversedHex(valStr));
                            j++;
                        }
                    }
                    break;
                }
            }
        }
        return newKipdata;
    }

    virtual bool handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) override {
        if (keysDown & KEY_B) {
            tsl::goBack();
            return true;
        }
         if (keysDown & KEY_A) {
            std::map <std::string,std::string> offsetData = parseJson(jsonPath, specficKey);
            hexEditByOffsetF(kipPath, offsetData);
            applied = true;
            tsl::goBack();
            return true;
         }
        return false;
    }
};