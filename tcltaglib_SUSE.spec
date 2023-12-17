%{!?directory:%define directory /usr}

%define buildroot %{_tmppath}/%{name}

Name:          tcltaglib
Summary:       Tcl interface for tablib
Version:       1.1
Release:       0
License:       BSD-3-Clause
Group:         Development/Libraries/Tcl
Source:        %name-%version.tar.gz
URL:           https://github.com/ray2501/tcltaglib
BuildRequires: autoconf
BuildRequires: make
BuildRequires: tcl-devel >= 8.4
BuildRequires: libtag-devel
Requires: tcl >= 8.4
Requires: libtag1
BuildRoot:     %{buildroot}

%description
Tcl interface for taglib (Abstract API only).

%prep
%setup -q -n %{name}-%{version}

%build
CFLAGS="%optflags" ./configure \
	--prefix=%{directory} \
	--exec-prefix=%{directory} \
	--libdir=%{directory}/%{_lib}
make 

%install
make DESTDIR=%{buildroot} pkglibdir=%{tcl_archdir}/%{name}%{version} install

%clean
rm -rf %buildroot

%files
%defattr(-,root,root)
%{tcl_archdir}
