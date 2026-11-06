// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Techniix (Ship of Harkinian Haiku port)
#ifdef __HAIKU__
#include "HaikuFilePanel.h"

#include <Application.h>
#include <Entry.h>
#include <FilePanel.h>
#include <Looper.h>
#include <Message.h>
#include <Messenger.h>
#include <Node.h>
#include <OS.h>
#include <Path.h>
#include <Window.h>

#include <string.h>
#include <strings.h>

namespace SohHaiku {

namespace {

bool HasRomExtension(const char* name) {
    const char* ext = strrchr(name, '.');
    if (ext == nullptr) {
        return false;
    }
    return strcasecmp(ext, ".z64") == 0 || strcasecmp(ext, ".n64") == 0 || strcasecmp(ext, ".v64") == 0;
}

class RomRefFilter : public BRefFilter {
  public:
    bool Filter(const entry_ref* ref, BNode* node, struct stat_beos* stat, const char* mimeType) override {
        (void)stat;
        (void)mimeType;

        if (node->IsDirectory()) {
            return true;
        }

        if (node->IsSymLink()) {
            // Show links to folders and links to ROMs.
            BEntry target(ref, true);
            if (target.InitCheck() != B_OK) {
                return false;
            }
            if (target.IsDirectory()) {
                return true;
            }
            char name[B_FILE_NAME_LENGTH];
            return target.GetName(name) == B_OK && HasRomExtension(name);
        }

        return HasRomExtension(ref->name);
    }
};

// Receives the panel's reply and wakes the waiting thread.
class PanelReplyLooper : public BLooper {
  public:
    explicit PanelReplyLooper(sem_id doneSem) : BLooper("soh rom panel"), mDoneSem(doneSem) {
    }

    void MessageReceived(BMessage* message) override {
        switch (message->what) {
            case B_REFS_RECEIVED: {
                entry_ref ref;
                if (message->FindRef("refs", &ref) == B_OK) {
                    BPath path(&ref);
                    if (path.InitCheck() == B_OK) {
                        mPath = path.Path();
                    }
                }
                Finish();
                break;
            }
            case B_CANCEL:
                // Also sent when the panel hides after a pick; Finish() only signals once.
                Finish();
                break;
            default:
                BLooper::MessageReceived(message);
                break;
        }
    }

    const std::string& PickedPath() const {
        return mPath;
    }

  private:
    void Finish() {
        if (!mFinished) {
            mFinished = true;
            release_sem(mDoneSem);
        }
    }

    sem_id mDoneSem;
    bool mFinished = false;
    std::string mPath;
};

} // namespace

bool OpenRomFilePanel(const std::string& startDir, std::string& outPath) {
    // SDL's video subsystem starts a BApplication; the panel needs it to talk to Tracker.
    if (be_app == nullptr) {
        return false;
    }

    sem_id doneSem = create_sem(0, "soh rom panel done");
    if (doneSem < B_OK) {
        return false;
    }

    PanelReplyLooper* looper = new PanelReplyLooper(doneSem);
    looper->Run();

    BMessenger target(looper);
    RomRefFilter filter;

    entry_ref startRef;
    const entry_ref* startRefPtr = nullptr;
    if (!startDir.empty() && get_ref_for_path(startDir.c_str(), &startRef) == B_OK) {
        startRefPtr = &startRef;
    }

    BFilePanel* panel =
        new BFilePanel(B_OPEN_PANEL, &target, startRefPtr, B_FILE_NODE, false, nullptr, &filter, true, true);

    if (panel->Window()->Lock()) {
        panel->Window()->SetTitle("Ship of Harkinian: Select an Ocarina of Time ROM");
        panel->Window()->Unlock();
    }
    panel->SetButtonLabel(B_DEFAULT_BUTTON, "Open");
    panel->Show();

    status_t err;
    do {
        err = acquire_sem(doneSem);
    } while (err == B_INTERRUPTED);

    delete panel;

    std::string picked;
    if (looper->Lock()) {
        picked = looper->PickedPath();
        looper->Quit();
    }

    delete_sem(doneSem);

    if (picked.empty()) {
        return false;
    }

    outPath = picked;
    return true;
}

} // namespace SohHaiku
#endif
