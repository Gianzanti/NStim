void screenMonitor() {
    int picId = Lcm.readPicId();
    if (picId != 0xFFFF && picId != currentScreen) {
        switch (picId) {
            case PID_INTRO:
                break;

            case PID_BR_MAIN:
            case PID_EN_MAIN:
                break;

            case PID_BR_CONFIG_WAVE_POLE:
            case PID_EN_CONFIG_WAVE_POLE:
                break;

            case PID_BR_CONFIG_WAVE_PHASE:
            case PID_EN_CONFIG_WAVE_PHASE:
                break;

            case PID_BR_CONFIG_WAVE_TRAIN:
            case PID_EN_CONFIG_WAVE_TRAIN:
                break;

            case PID_BR_CONFIG_WAVE_TRAIN_DETAILS_PULSE:
            case PID_EN_CONFIG_WAVE_TRAIN_DETAILS_PULSE:
                break;

            case PID_BR_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL:
            case PID_EN_CONFIG_WAVE_TRAIN_DETAILS_INTERVAL:
                break;

            case PID_BR_CONFIG_WAVE_TRAIN_DETAILS_EXIT:
            case PID_EN_CONFIG_WAVE_TRAIN_DETAILS_EXIT:
                break;

            case PID_BR_CONFIG_WAVE_TRAIN_EXIT:
            case PID_EN_CONFIG_WAVE_TRAIN_EXIT:
                break;

            case PID_BR_CONFIG_CURRENT:
            case PID_EN_CONFIG_CURRENT:
                break;

            case PID_BR_CONFIG_PERIOD:
            case PID_EN_CONFIG_PERIOD:
                break;

            case PID_BR_CONFIG_FREQUENCY:
            case PID_EN_CONFIG_FREQUENCY:
                break;

            case PID_BR_CONFIG_GENERAL_USER:
            case PID_EN_CONFIG_GENERAL_USER:
                break;

            case PID_BR_CONFIG_GENERAL_USER_LANG:
            case PID_EN_CONFIG_GENERAL_USER_LANG:
                break;

            case PID_BR_CONFIG_GENERAL_USER_SOUND:
            case PID_EN_CONFIG_GENERAL_USER_SOUND:
                break;

            case PID_BR_CONFIG_GENERAL_USER_BACKLIGHT:
            case PID_EN_CONFIG_GENERAL_USER_BACKLIGHT:
                break;

            case PID_BR_CONFIG_GENERAL_USER_EXIT:
            case PID_EN_CONFIG_GENERAL_USER_EXIT:
                break;

            case PID_BR_CONFIG_GENERAL_MODE:
            case PID_EN_CONFIG_GENERAL_MODE:
                break;

            case PID_BR_CONFIG_GENERAL_MODE_CORTICAL:
            case PID_EN_CONFIG_GENERAL_MODE_CORTICAL:
                break;

            case PID_BR_CONFIG_GENERAL_MODE_LOCALIZATION:
            case PID_EN_CONFIG_GENERAL_MODE_LOCALIZATION:
                break;

            case PID_BR_CONFIG_GENERAL_MODE_EXIT:
            case PID_EN_CONFIG_GENERAL_MODE_EXIT:
                break;

            case PID_BR_CONFIG_GENERAL_EXIT:
            case PID_EN_CONFIG_GENERAL_EXIT:
                break;

            default:
                break;
        }
    }
}
