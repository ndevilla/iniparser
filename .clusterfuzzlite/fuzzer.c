#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"
#include "iniparser.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Create a temporary file with the fuzzed data
    FILE *temp_file = tmpfile();
    if (temp_file == NULL) {
        return 0;
    }

    // Write the fuzzed data to the temporary file
    fwrite(data, 1, size, temp_file);
    fseek(temp_file, 0, SEEK_SET);

    // Call the target function with the temporary file and a dummy filename
    const char dummy_filename[] = "dummy.ini";
    dictionary *result = iniparser_load_file(temp_file, dummy_filename);

    // Cleanup
    if (result != NULL) {
        dictionary_del(result);
    }
    fclose(temp_file);

    return 0;
}
  
