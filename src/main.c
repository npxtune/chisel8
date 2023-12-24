#include "gui/main_window.h"
#include "gui/options_window.h"

int main(void) {
    options_config options;
    load_settings(&options);
    main_window(&options);
    return 0;
}