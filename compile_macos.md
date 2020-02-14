## Create custom build

mkdir build && cd build

../autogen.sh

export PATH="/usr/local/opt/gettext/bin:/usr/local/opt/ccache/libexec:/usr/local/opt/rpcgen/bin:$PATH"
export PKG_CONFIG_PATH="/usr/local/opt/libxml2/lib/pkgconfig"

../configure --prefix=/usr/local/Cellar/libvirt/6.0.0 --localstatedir=/usr/local/var --mandir=/usr/local/Cellar/libvirt/6.0.0/share/man --sysconfdir=/usr/local/etc --with-qemu=yes --with-esx=no --with-network=yes --with-hyperv=no --with-macvtap=yes gl_cv_func_ftello_works=yes

make -j3 && make -j3 install

## Undefine vagrant-libvirt network after breaking boot

```
sudo virsh net-undefine vagrant-libvirt
```

## The test
 
 - boot libvirtd

sudo libvirtd -f ~/Development/libvirt/libvirt_macos.conf -v

- Create network

sudo virsh net-create ~/Development/libvirt/example_network.xml
