Name:           readahead-utils
Version:        0.1.2
Release:        1%{?dist}
Summary:        Utility to set the readahead of a mountpoint.

License:        MIT
URL:            https://github.com/trbecker/readahead-utils
Source0:        %{name}-%{version}.tgz
Source1:	readahead.conf

BuildRequires:  flex bison libmount-devel
Requires:       libmount

%description
Utility to set readahead at mountpoint.

%prep
%autosetup
./autogen.sh

%build
%configure
%make_build

%install
rm -rf $RPM_BUILD_ROOT
%make_install
cp %{SOURCE1} ${RPM_BUILD_ROOT}%{_sysconfdir}


%files
%license LICENSE
%doc README.md

%attr(0755,root,root)	%{_sbindir}/udev_read_ahead_helper

%config(noreplace)	%{_sysconfdir}/readahead.conf
# Must replace
%{_sysconfdir}/udev/rules.d/99-bdi.rules


%changelog
* Thu Aug 19 2021 Thiago Rafael Becker <tbecker@gmail.com> - 0.1.1
- Fix compilation warnings and errors for RPM packaging

* Wed Aug 18 2021 Thiago Rafael Becker <tbecker@gmail.com> - 0.1
- Initial release (Thiago Rafael Becker)
