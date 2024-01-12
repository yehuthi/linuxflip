#pragma once
#include <stdbool.h>

/// Internal and unstable data.
struct linuxflip {
	/// The udev instance. We keep it so we can clean it up later.
	struct udev* udev;
	/// The libinput instance.
	struct libinput* libinput;
};

/// \brief The mode (tablet mode / laptop mode).
///
/// The mode, which can be #LINUXFLIP_STATE_ON (which seems to indicate tablet mode) or
/// #LINUXFLIP_STATE_OFF (which seems to indicate laptop mode).
typedef unsigned char linuxflip_state;

/// Off state, seemingly indicates laptop mode.
#define LINUXFLIP_STATE_OFF ((linuxflip_state)0)
/// On state, seemingly indicates tablet mode.
#define LINUXFLIP_STATE_ON  ((linuxflip_state)1)

/// \brief Hook that will be called when the #linuxflip_state changes.
/// \param state The new #linuxflip_state.
/// \param data User data (passed via #linuxflip_go).
/// \returns Whether to keep waiting for more state changes.
typedef bool (*linuxflip_hook)(linuxflip_state state, void* data);

/// Instantiates a new #linuxflip.
struct linuxflip linuxflip_new(void);

/// \brief Listen to #linuxflip_state changes.
///
/// Listens for #linuxflip_state changes and calls the given hook.
/// This function blocks.
///
/// \param linuxflip linuxflip The #linuxflip instance.
/// \param hook The #linuxflip_hook that will be called.
/// \param data Any arbitrary user data you want the hook to have access to.
void linuxflip_go(struct linuxflip linuxflip, linuxflip_hook hook, void* data);

/// \brief Frees resources used by #linuxflip.
void linuxflip_free(struct linuxflip linuxflip);
