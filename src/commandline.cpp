// Command-line parser (0x4bc210-0x4bc35f).
#include "rallytrophy.h"

// FUNCTION: RALLYTROPHY 0x4bc210
CommandLine::CommandLine(const char* commandLine)
{
    m_count = 0;
    m_buffer = 0;
    Parse(commandLine);
}

// FUNCTION: RALLYTROPHY 0x4bc240
CommandLine::~CommandLine()
{
    delete m_buffer;
}

// FUNCTION: RALLYTROPHY 0x4bc250
// Splits the command line on whitespace; "quoted arguments" may contain spaces.
void CommandLine::Parse(const char* commandLine)
{
    if (!commandLine) {
        return;
    }
    int length = strlen(commandLine);
    m_buffer = new char[length + 1];
    memcpy(m_buffer, commandLine, length + 1);

    int start = 0;
    int inArg = 0;
    int quoted = 0;
    for (int i = 0; i < length; i++) {
        char c = m_buffer[i];
        int space = 0;
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            space = 1;
        }
        if (!inArg) {
            if (!space) {
                inArg = 1;
                start = i;
                quoted = c == '"';
            }
        } else if (space) {
            if (!quoted) {
                m_buffer[i] = 0;
                m_args[m_count] = m_buffer + start;
                m_count++;
                inArg = 0;
                quoted = 0;
            }
        } else if (quoted && c == '"') {
            m_buffer[i] = 0;
            m_args[m_count] = m_buffer + start + 1;
            m_count++;
            inArg = 0;
            quoted = 0;
        }
    }
    if (inArg) {
        if (quoted) {
            start++;
        }
        m_args[m_count] = m_buffer + start;
        m_count++;
    }
}
