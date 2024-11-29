#include "door_lock_system.h"
#include <Crypto.h>
#include <SHA256.h>

void hashString(byte outputBuffer[32], const String& input) {
  SHA256 sha256;
  sha256.update(input.c_str(), input.length());
  sha256.finalize(outputBuffer, 32);
}