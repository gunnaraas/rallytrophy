// Renderer plugin loader T_56b980 (0x4bbdf0-0x4bc1db).
#include "rallytrophy.h"

typedef List<RenderDevice*> RenderDeviceList;
typedef List<Plugin*> PluginList;

// FUNCTION: RALLYTROPHY 0x4aecf0
// SYMBOL: ?AddTail@?$List@PAVRenderDevice@@@@QAEXPAVRenderDevice@@@Z
template<> void RenderDeviceList::AddTail(RenderDevice* item)
{
    AdapterNode* node = m_free;
    if (!node) {
        node = (AdapterNode*)operator new(sizeof(AdapterNode));
        if (node) {
            node->prev = 0;
            node->next = 0;
            node->data = 0;
        } else {
            node = 0;
        }
    } else {
        m_free = node->next;
        m_freeCount--;
    }
    AdapterNode* tail = m_tail;
    node->data = item;
    node->prev = tail;
    node->next = 0;
    if (tail) {
        m_tail = node;
        tail->next = node;
        m_count++;
        return;
    }
    m_head = node;
    m_tail = node;
    m_count++;
}

template<> void PluginList::AddTail(Plugin* item)
{
    Node* node = m_free;
    if (!node) {
        node = (Node*)operator new(sizeof(Node));
        if (node) {
            node->prev = 0;
            node->next = 0;
            node->data = 0;
        } else {
            node = 0;
        }
    } else {
        m_free = node->next;
        m_freeCount--;
    }
    Node* tail = m_tail;
    node->data = item;
    node->prev = tail;
    node->next = 0;
    if (tail) {
        m_tail = node;
        tail->next = node;
        m_count++;
        return;
    }
    m_head = node;
    m_tail = node;
    m_count++;
}

// FUNCTION: RALLYTROPHY 0x414490
// SYMBOL: ?RemoveNode@?$List@PAVRenderDevice@@@@QAEXPAUNode@1@@Z
template<> void RenderDeviceList::RemoveNode(AdapterNode* node)
{
    Remove(node);
}

template<> void PluginList::RemoveNode(Node* node)
{
    Remove(node);
}

// FUNCTION: RALLYTROPHY 0x4bbdf0
T_56b980::T_56b980()
{
}

// FUNCTION: RALLYTROPHY 0x4bbe20
T_56b980::~T_56b980()
{
    FUN_004bc120();
    while (m_plugins.m_head) {
        List<Plugin*>::Node* node = m_plugins.m_head;
        if (node->data) {
            delete node->data;
        }
        m_plugins.RemoveNode(node);
    }
    while (m_devices.m_head) {
        AdapterNode* node = m_devices.m_head;
        delete node->data;
        m_devices.RemoveNode(node);
    }
}

// FUNCTION: RALLYTROPHY 0x4bbf60
// Loads a renderer plugin DLL and adds its devices to the device list.
int T_56b980::FUN_004bbf60(const char* dll)
{
    HMODULE module = LoadLibraryA(dll);
    FUN_004a7d30();
    if (!module) {
        return 0;
    }
    BeGetDevicesFunc getDevices = (BeGetDevicesFunc)GetProcAddress(module, "BeGetDevices");
    if (!getDevices) {
        FreeLibrary(module);
        return 0;
    }
    Plugin* plugin = new Plugin;
    List<RenderDevice*>* devices = getDevices();
    plugin->module = module;
    plugin->devices = devices;
    for (AdapterNode* node = devices->m_head; node; node = node->next) {
        m_devices.AddTail(node->data);
    }
    m_plugins.AddTail(plugin);
    return m_devices.m_count;
}

// FUNCTION: RALLYTROPHY 0x4bbff0
// Keeps only the plugin that provides `device` and drops all other devices.
// Returns 0 if no loaded plugin provides it.
int T_56b980::FUN_004bbff0(RenderDevice* device)
{
    List<Plugin*>::Node* plugin;
    AdapterNode* entry;
    for (plugin = m_plugins.m_head; plugin; plugin = plugin->next) {
        for (entry = plugin->data->devices->m_head; entry; entry = entry->next) {
            if (entry->data == device) {
                break;
            }
        }
        if (entry) {
            goto found;
        }
    }
    return 0;
found:

    AdapterNode* node = m_devices.m_head;
    while (node) {
        AdapterNode* next = node->next;
        if (node->data != entry->data) {
            m_devices.Remove(node);
        }
        node = next;
    }

    List<Plugin*>::Node* other = m_plugins.m_head;
    while (other) {
        List<Plugin*>::Node* next = other->next;
        if (other != plugin) {
            // Bug in the original: unloads the selected plugin's module, not other's.
            FreeLibrary(plugin->data->module);
            if (other->data) {
                delete other->data;
            }
            m_plugins.Remove(other);
        }
        other = next;
    }
    return 1;
}

// FUNCTION: RALLYTROPHY 0x4bc120
// Forgets all devices and unloads every plugin DLL.
void T_56b980::FUN_004bc120()
{
    m_devices.RemoveAll();
    List<Plugin*>::Node* node = m_plugins.m_head;
    while (node) {
        List<Plugin*>::Node* next = node->next;
        FreeLibrary(node->data->module);
        if (node->data) {
            delete node->data;
        }
        m_plugins.Remove(node);
        node = next;
    }
}
