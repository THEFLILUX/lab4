#pragma once

namespace utec {
namespace spatial {

class SpatialImageBase {
 public:
  virtual void load(const std::string& filename) = 0;

  /* Comprime la imagen previamente cargada desde disco y la almacena en disco
    filename: nombre del archivo que almacenará la imagen comprimida
  */
  virtual void compress(const std::string& filename) = 0;

  /* Descomprime una imagen desde disco y la almacena en memoria
    filename: nombre del archivo a descomprimir
  */
  virtual void decompress(const std::string& filename) = 0;

  /* Almacena la representación de la imagen que tiene el QuadTree
     en formato de imagen.
    filename: nombre del archivo en disco donde la imagen será almacenada
  */
  virtual void convertToPGM(const std::string& filename) = 0;
};

}  // namespace spatial
}  // namespace utec