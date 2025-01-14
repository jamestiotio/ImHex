#pragma once

#include <hex/ui/view.hpp>

#include <ui/hex_editor.hpp>

namespace hex::plugin::builtin {

    class ViewHexEditor : public View::Window {
    public:
        ViewHexEditor();
        ~ViewHexEditor() override;

        void drawContent() override;
        [[nodiscard]] ImGuiWindowFlags getWindowFlags() const override {
            return ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        }

        class Popup {
        public:
            virtual ~Popup() = default;
            virtual void draw(ViewHexEditor *editor) = 0;
        };

        [[nodiscard]] bool isAnyPopupOpen() const {
            return m_currPopup != nullptr;
        }

        template<std::derived_from<Popup> T>
        [[nodiscard]] bool isPopupOpen() const {
            return dynamic_cast<T*>(m_currPopup.get()) != nullptr;
        }

        template<std::derived_from<Popup> T, typename ... Args>
        void openPopup(Args && ...args) {
            m_currPopup = std::make_unique<T>(std::forward<Args>(args)...);
            m_shouldOpenPopup = true;
        }

        void closePopup() {
            m_currPopup.reset();
        }

        bool isSelectionValid() const {
            return m_hexEditor.isSelectionValid();
        }

        Region getSelection() const {
            return m_hexEditor.getSelection();
        }

        void setSelection(const Region &region) {
            m_hexEditor.setSelection(region);
        }

        void setSelection(u128 start, u128 end) {
            m_hexEditor.setSelection(start, end);
        }

        void jumpToSelection() {
            m_hexEditor.jumpToSelection();
        }

    private:
        void drawPopup();

        void registerShortcuts();
        void registerEvents();
        void registerMenuItems();

        ui::HexEditor m_hexEditor;

        bool m_shouldOpenPopup = false;
        std::unique_ptr<Popup> m_currPopup;

        PerProvider<std::optional<u64>> m_selectionStart, m_selectionEnd;
        PerProvider<float> m_scrollPosition;

        PerProvider<std::map<u64, color_t>> m_foregroundHighlights, m_backgroundHighlights;
    };

}