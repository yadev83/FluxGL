#pragma once

namespace fluxgl {
    class Texture {
        private:
            unsigned int m_ID;
            int m_width, m_height, m_channels;

            void load(const char *data, int width, int height, int channels);

        public:
            Texture() = default;
            ~Texture();

            void bind(unsigned int slot = 0) const; 
            void unbind() const; 
            
            inline int getWidth() const { return m_width; } 
            inline int getHeight() const { return m_height; }
            inline int getChannels() const { return m_channels; }

            // Delete copy constructor and copy assignment operator to prevent copying 
            Texture(const Texture&) = delete; 
            Texture& operator=(const Texture&) = delete;
            // Instead, allow move
            Texture(Texture&& other) noexcept;
            Texture& operator=(Texture&& other) noexcept;

            // Builders
            static Texture fromFile(const char *path);
            static Texture fromMemory(const char *data, int width, int height, int channels); 
    };
}