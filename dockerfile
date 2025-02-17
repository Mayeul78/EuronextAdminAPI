# Utilisation d'une image Debian légère
FROM debian:latest

# Installer les dépendances requises
RUN apt-get update && apt-get install -y \
    g++ cmake make libboost-all-dev sqlite3 libsqlite3-dev git

# Installer Asio (sans Boost)
RUN mkdir -p /usr/local/include/asio && \
    git clone --depth=1 https://github.com/chriskohlhoff/asio.git /tmp/asio && \
    cp -r /tmp/asio/asio/include/asio /usr/local/include/ && \
    rm -rf /tmp/asio

# Installer Crow
RUN mkdir -p /usr/local/include/crow && \
    git clone --depth=1 https://github.com/CrowCpp/Crow.git /tmp/crow && \
    cp -r /tmp/crow/include/crow /usr/local/include/ && \
    rm -rf /tmp/crow

# Définir le répertoire de travail pour le projet
WORKDIR /app

# Copier les fichiers du projet
COPY . .

# Supprimer le cache CMake s'il existe déjà pour éviter l'erreur
RUN rm -rf build CMakeCache.txt CMakeFiles

# Créer le dossier de build et compiler proprement
RUN mkdir -p build && cd build && \
    cmake .. -DCMAKE_FORCE_REGENERATION=ON -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Exposer le port du serveur
EXPOSE 8080

# Lancer l'exécutable
CMD ["./build/TestServer"]
