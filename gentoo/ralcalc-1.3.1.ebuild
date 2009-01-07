# Copyright 1999-2008 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit eutils

DESCRIPTION="A command line calculator with SI prefix support."
HOMEPAGE="http://atchoo.org/tools/ralcalc/"
SRC_URI="http://atchoo.org/tools/ralcalc/files/${P}.tar.bz2"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~x86"
IUSE=""

DEPEND="sys-devel/gettext
	>=dev-util/intltool-0.35"
RDEPEND="${DEPEND}"

src_compile() {
	sed -i 's#/usr/local#/usr#' config.mk
	emake || die "emake failed"
}

src_install() {
	emake DESTDIR="${D}" install || die "emake install failed"
}
