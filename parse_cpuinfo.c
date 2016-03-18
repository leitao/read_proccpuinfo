#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>

#define MAX_LINELENGTH 1024

static int64_t* get_cpu_frequency_from_file(const char *file, int ncpus)
{

  std::ifstream cpuinfo(file);
  if (cpuinfo.fail()) {
    return nullptr;
  }
  char line[MAX_LINELENGTH];
  int64_t* freqs = new int64_t[ncpus];
  for (int i = 0; i < ncpus; ++i) {
    freqs[i] = 0;
  }
  int processor = -1;

  while (cpuinfo.getline(line, sizeof(line))) {
    if (sscanf(line, "processor : %d", &processor) == 1) {
      continue;
    }
    float freq;
    if ((sscanf(line, "cpu MHz : %f", &freq) == 1) ||
        (sscanf(line, "clock         : %f", &freq) == 1)) {
      if (processor != -1 && processor < ncpus) {
         freqs[processor] = (int) freq;
         processor = -1;
      }
    }
  }
  for (int i = 0; i < ncpus; ++i) {
    if (freqs[i] == 0) {
      delete[] freqs;
      return nullptr;
    }
  }
  return freqs;
}

int main(){
  int64_t *a;
  int ncpus = 8;

  a = get_cpu_frequency_from_file("/proc/cpuinfo", ncpus);

  for (int i =0 ; i < ncpus; i++)
    std::cout << a[i] << std::endl;

  delete(a);
  return 0;
}
