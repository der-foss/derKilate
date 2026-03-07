#include "kilate/file.h"

#include <stdio.h>
#include <stdlib.h>

#include "kilate/error.h"
#include "kilate/string.h"

// Opens a file
// After do all uses with file, close it
// with file_close
file* file_open(str filePath, str mode) {
  file* file = malloc(sizeof(file));
  if (file == NULL) {
    error_fatal("Failed to open %s", filePath);
    return NULL;
  }
  file->std_file = fopen(filePath, mode);
  if (file->std_file == NULL) {
    error_fatal("Failed to open %s", filePath);
    return NULL;
  }
  file->path = filePath;
  return file;
}

void file_close(file* file) {
  fclose(file->std_file);
  free(file);
}

// Returns the length of file content.
size_t file_get_length(file* file) {
  fseek(file->std_file, 0, SEEK_END);
  size_t len = ftell(file->std_file);
  rewind(file->std_file);
  return len;
}

// Reads the content of file.
// Result should be free.
str file_read_text(file* file) {
  if (file == NULL || file->std_file == NULL) {
    error_fatal("Failed to allocate memory to open file.");
    return NULL;
  }
  size_t fileLen = file_get_length(file);
  str buffer = malloc(fileLen + 1);  // 1 for null-terminate
  if (buffer == NULL) {
    error_fatal("Failed to allocate memory to read file %s", file->path);
    return NULL;
  }
  fread(buffer, 1, fileLen, file->std_file);  // read file into buffer
  buffer[fileLen] = '\0';                     // null-terminate.
  return buffer;
}