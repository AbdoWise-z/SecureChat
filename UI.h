//
// Created by xAbdoMo on 4/14/2024.
//

#ifndef SECURITY_PROJECT_UI_H
#define SECURITY_PROJECT_UI_H


namespace UI {
    void InitUI();
    void DrawUI(bool&);
    void Terminate();
    void message(const char* ptr);
    void info(const char* ptr);
    void error(const char* ptr);
};


#endif //SECURITY_PROJECT_UI_H
