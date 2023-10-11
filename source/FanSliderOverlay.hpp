#define NDEBUG
#define STBTT_STATIC
#define TESLA_INIT_IMPL
#include <tesla.hpp>
#include <utils.hpp>

class FanSliderOverlay : public tsl::Gui {
private:
    std::string filePath, specificKey, pathPattern, pathPatternOn, pathPatternOff, jsonPath, jsonKey, itemName, parentDirName, lastParentDirName, textPath;
    std::vector<std::string> filesList, filesListOn, filesListOff, filterList, filterOnList, filterOffList;
    std::vector<std::vector<std::string>> commands;
    bool toggleState = false;
    json_t* jsonData;

public:
    FanSliderOverlay(const std::string& file, const std::string& key = "", const std::vector<std::vector<std::string>>& cmds = {}) 
        : filePath(file), specificKey(key), commands(cmds) {}
    ~FanSliderOverlay() {}

    virtual tsl::elm::Element* createUI() override {
        // logMessage ("FanSliderOverlay");

        size_t fifthSlashPos = filePath.find('/', filePath.find('/', filePath.find('/', filePath.find('/') + 1) + 1) + 1);
        bool hasHelp = false;
        std::string helpPath = "";
        std::string menuName = "";
        if (fifthSlashPos != std::string::npos) {
            // Extract the substring up to the fourth slash
            helpPath = filePath.substr(0, fifthSlashPos);
            if (!specificKey.empty()) {
                menuName = specificKey;
                removeLastNumericWord(menuName);
                helpPath += "/Help/" + getNameWithoutPrefix(getNameFromPath(filePath)) + "/" + menuName.substr(1) + ".txt";
            } else {
                helpPath += "/Help/" + getNameWithoutPrefix(getNameFromPath(filePath)) + ".txt";
            }
            if (isFileOrDirectory(helpPath)) {
               hasHelp = true; 
            } else {
                helpPath = "";
            }

        }
        auto rootFrame = new tsl::elm::OverlayFrame(specificKey.empty() ? getNameWithoutPrefix(getNameFromPath(filePath)) : specificKey.substr(1),
                                                    "Uberhand Package", "", hasHelp, "\uE0E1  Back     \uE0E0  Apply");
        auto list = new tsl::elm::List();

        bool useSlider = false;
        std::string sourceIni = "";
        std::string sectionIni = "";
        std::string keyIni = "";
        std::pair<std::string, int> textDataPair;

        constexpr int lineHeight = 20;  // Adjust the line height as needed
        constexpr int fontSize = 19;    // Adjust the font size as needed
        
        for (const auto& cmd : commands) {
            if (cmd.size() > 1) {
                if (cmd[0] == "slider_ini") {
                    sourceIni  = preprocessPath(cmd[1]);
                    useSlider = true;
                }
            } 
        }

        // Get the list of files matching the pattern
        if (useSlider) {
            if (!isFileOrDirectory(sourceIni)) {
                list->addItem(new tsl::elm::CustomDrawer([lineHeight, fontSize](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h) {
                    renderer->drawString("INI file not found.\nContact the package dev.", false, x, y + lineHeight, fontSize, a(tsl::style::color::ColorText));
                    }), fontSize + lineHeight);
                    rootFrame->setContent(list);
                    return rootFrame;
            } else {
                std::string iniString = readIniValue(sourceIni, "tc", "tskin_rate_table_console_on_fwdbg");
                std::vector<std::vector<int>> iniValues = parseIntIniData(iniString);
                // logMessage(readIniValue("/atmosphere/config/system_settings.ini", "tc", "tskin_rate_table_console_on_fwdbg"));
                // logMessage(std::to_string(iniValues.size()));
                for (const auto& arr : iniValues) {
                    std::string low = arr[0] < 0 ? "0" : std::to_string(arr[0]/1000) + "°C";
                    std::string high = arr[1] > 100000 ? "100°C" : std::to_string((arr[1]/1000) - 1) + "°C";
                    std::string header = "Fan speed at " + low + " - " + high + ": ";
                    // auto catHeader = new tsl::elm::CustomHeader(header);
                    // list->addItem(catHeader);

                    double stepSize = 0.05 * 255;
                    int percentage = 0;
                    // logMessage(std::to_string(arr[3]));
                    if (arr[3] > 0) {
                        percentage = static_cast<int>(ceil(arr[3] / stepSize));
                    }
                    // logMessage(std::to_string((percentage)));
                    // logMessage("end");

                    auto slider = new tsl::elm::NamedStepTrackBar(" ",{header + "0%", header + "5%", header + "10%", header + "15%", header + "20%", header + "25%", header + "30%", header + "35%", header + "40%", header + "45%", header + "50%", header + "55%", header + "60%", header + "65%", header + "70%", header + "75%", header + "80%", header + "85%", header + "90%", header + "95%", header+"100%"});

                    slider->setProgress(percentage);
                    slider->setClickListener([this, list, iniString, sourceIni, iniValues](uint64_t keys) { // Add 'command' to the capture list
                        if (keys & KEY_A) {
                            std::vector<int> values;
                            size_t listSize = list->getSize();
                            for (size_t i = 0; i < listSize; i++) {
                                if (list->getItemAtIndex(i)->getClass()  == "TrackBar") {
                                    // logMessage(std::to_string(int(double(dynamic_cast<tsl::elm::StepTrackBar*>(list->getItemAtIndex(i))->getProgress())/100*255)));
                                    values.push_back(int(double(dynamic_cast<tsl::elm::StepTrackBar*>(list->getItemAtIndex(i))->getProgress())*12.75));
                                }
                            }
                            setIniFileValue(sourceIni, "tc", "tskin_rate_table_console_on_fwdbg", formString(values, parseIntIniData(iniString, false)));
                            setIniFileValue(sourceIni, "tc", "tskin_rate_table_handheld_on_fwdbg", formString(values, parseIntIniData(iniString, false)));
                            // logMessage(std::to_string(listSize));
                            // logMessage(this->getFocusedElement()->getClass());
                            applied = true;
                            tsl::goBack();
                            return true;
                        }
                        return false;
                    });
                    list->addItem(slider);
                }
                rootFrame->setContent(list);
            }
        }
        return rootFrame;
    }

    virtual bool handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) override {
        if (keysDown & KEY_B) {
            tsl::goBack();
            return true;
        } else if ((applied || deleted) && !keysDown) {
            deleted = false;
            tsl::elm::ListItem* focusedItem = dynamic_cast<tsl::elm::ListItem*>(this->getFocusedElement());
            if (prevValue.empty())
                prevValue = focusedItem->getValue();
            if (applied) {
                resetValue = true;
                focusedItem->setValue("APPLIED", tsl::PredefinedColors::Green);
            }
            else
                focusedItem->setValue("DELETED", tsl::PredefinedColors::Red);
            applied = false;
            deleted = false;
            return true;
        } else if (resetValue && keysDown) {
            if (this->getFocusedElement()->getClass()  == "ListItem" ){
                tsl::elm::ListItem* focusedItem = dynamic_cast<tsl::elm::ListItem*>(this->getFocusedElement());
                if (focusedItem->getClass() == "ListItem" && focusedItem->getValue() == "APPLIED") {
                    focusedItem->setValue(prevValue);
                    prevValue = "";
                    resetValue = false;
                }
            }
            return true;
        }
        return false;
    }

    std::string formString(std::vector<int> newValues, std::vector<std::vector<int>> initialData) {

        std::stringstream result;
        result << "\"str!\"[[";

        newValues.insert(newValues.begin(), 0);

        logMessage(std::to_string(newValues.size()));
        logMessage(std::to_string(initialData.size()));

        size_t newValueIndex = 0;
        int newThird = -1;

        for (size_t i = 0; i < initialData.size(); i++) {
            std::vector<int> arr = initialData[i];
            if (newThird >= 0)
                arr[2] = newThird;
            if (newValues[newValueIndex] < arr[2])
                arr[3] = arr[2];
            else
                arr[3] = newValues[newValueIndex];
            newThird = arr[3];
            newValueIndex++;
            for (int j = 0; j < 3; j++) {
                result << arr[j] << ", ";
            }
            if (i != initialData.size() -1)
                result << arr[3]  <<  "], [";
            else
                result << arr[3] << "]]\"\"";

        }
        logMessage(result.str());
        return result.str();
    }
};
