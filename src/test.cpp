int test(char* cmd[]) {

    int result;
    string fullPath;

    fullPath = "/home/csmajs/wha004/rshell/src/test.sh ";

    if (strcmp(cmd[1], "-e") == 0 || strcmp(cmd[1], "-f") == 0 || strcmp(cmd[1], "-d") == 0) {
        fullPath += cmd[1];
        fullPath += " ";
        fullPath += cmd[2];
    }else {
        fullPath += "-e ";
        
        if (strchr(cmd[1], '-') != 0) {
            if (cmd[2] != NULL) {
                fullPath += cmd[2];
            }else {
                return -1;
            }
        }else {
            fullPath += cmd[1];
        }
    }
    
    cout << "Fullpath is: " << fullPath << endl;
    
    result = system(fullPath.c_str());

    if (result == 0) {
        return 0;
    }else {
        return -1;
    }
}
