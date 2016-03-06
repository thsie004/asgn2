int fileCheck(string flag, string path) {
    struct stat check;
    int result = stat(path.c_str(), &check); 

    if (flag == "-e") {
        return result;   
        //cout << result << endl;
    }else if (flag == "-f") {
        if (S_ISREG(check.st_mode)) {
            return 0;
        }else {
            return -1;
        }
    }else if (flag == "-d") {
        if (S_ISDIR(check.st_mode)) {
            return 0;
        }else {
            return -1;
        }
    }

    return -1;
}

int test(char* cmd[]) {

    int result;
    string fullPath;
    string flag;

    if (strcmp(cmd[1], "-e") == 0 || strcmp(cmd[1], "-f") == 0 || strcmp(cmd[1], "-d") == 0) {
        flag = cmd[1];
        fullPath = cmd[2];
    }else {
        flag = "-e";
        
        if (strchr(cmd[1], '-') != 0) {
            if (cmd[2] != NULL) {
                fullPath = cmd[2];
            }else {
                return -1;
            }
        }else {
            fullPath = cmd[1];
        }
    }
    
    //cout << "Fullpath is: " << fullPath << endl;
    
    result = fileCheck(flag, fullPath);

    if (result == 0) {
        cout << "(True)" << endl;
        return 0;
    }else {
        cout << "(False)" << endl;
        return -1;
    }
}
