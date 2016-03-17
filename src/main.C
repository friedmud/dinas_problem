#include "libmesh/libmesh.h"
#include "libmesh/mesh.h"
#include "libmesh/node.h"
#include "libmesh/mesh_refinement.h"

using namespace libMesh;

bool sameNodes(const MeshBase & mesh1, const MeshBase & mesh2)
{
  if (mesh1.n_nodes() != mesh2.n_nodes())
    return false;

  for (unsigned int n=0; n < mesh1.n_nodes(); n++)
    if (mesh1.node(n)(0) != mesh2.node(n)(0) || mesh1.node(n)(1) != mesh2.node(n)(1) || mesh1.node(n)(2) != mesh2.node(n)(2))
    {
      std::cout<<"Nodes are different: \n"<<mesh1.node(n)<<"\n"<<mesh2.node(n)<<std::endl;
      return false;
    }
  return true;
}

int main (int argc, char ** argv)
{
  LibMeshInit init (argc, argv);

  Mesh mesh(init.comm());

  mesh.read("6patch_space5_coarse2.e");

  mesh.prepare_for_use();

  MeshRefinement mr(mesh);

  mr.uniformly_refine(2);

  mesh.print_info();

  mesh.write("mesh.xda");

  mesh.write("mesh.e");

  {
    Mesh remesh(init.comm());

    remesh.read("mesh.xda");

    remesh.write("remesh.e");

    remesh.prepare_for_use();

    if (sameNodes(mesh, remesh))
      std::cout<<"Same!"<<std::endl;
    else
      std::cout<<"Different!"<<std::endl;
  }

  return 0;
}
