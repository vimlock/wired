#pragma once

void wArgsAddFlag(const char *name);
void wArgsAddIntOption(const char *name);

int wArgsParse(int argc, const char **argv);

int wArgsGetNumPositional();
const char *wArgsGetPositional(int index);
