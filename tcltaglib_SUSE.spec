%{!?directory:%define directory /usr}

%define buildroot %{_tmppath}/%{name}

Name:          tcltaglib
Summary:       Tcl interface for tablib
Version:       0.9
Release:       2
License:       BSD
Group:         Development/Libraries/Tcl
Source:        https://sites.google.com/site/ray2501/tcltaglib/tcltaglib_0.9.zip
URL:           https://sites.google.com/site/ray2501/tcltaglib 
Buildrequires: libtag1 tcl >= 8.4
BuildRoot:     %{buildroot}

%description
Tcl interface for taglib (Abstract API only).

%prep
%setup -q -n %{name}

%build
CFLAGS="%optflags" ./configure \
	--prefix=%{directory} \
	--exec-prefix=%{directory} \
	--libdir=%{directory}/%{_lib}
make 

%install
make DESTDIR=%{buildroot} pkglibdir=%{directory}/%{_lib}/tcl/%{name}%{version} install

%clean
rm -rf %buildroot

%files
%defattr(-,root,root)
%{directory}/%{_lib}/tcl
