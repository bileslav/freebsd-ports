PORTNAME=	playmidi
PORTVERSION=	2.5
PORTREVISION=	2
CATEGORIES=	audio
MASTER_SITES=	SF

MAINTAINER=	ports@FreeBSD.org
COMMENT=	MIDI player
WWW=		https://sourceforge.net/projects/playmidi/

LICENSE=	GPLv2

USES=		cpe ncurses
WRKSRC=		${WRKDIR}/${PORTNAME}-2.4

ONLY_FOR_ARCHS=	i386 amd64
HAS_CONFIGURE=	yes
CONFIGURE_SCRIPT=	Configure
CONFIGURE_ENV=	PREFIX=${PREFIX}
INSTALL_TARGET=	install.lame install.man

OPTIONS_SINGLE=	DEFAULTDEVICE
OPTIONS_SINGLE_DEFAULTDEVICE=	EXTERNAL YAMAHA2 YAMAHA4 GRAVISULTRA SBAWE32

OPTIONS_DEFAULT=	EXTERNAL

EXTERNAL_DESC=	External midi synth, Ensoniq Soundscape, TB Maui, waveblaster cards
YAMAHA2_DESC=	Yamaha 2-op FM
YAMAHA4_DESC=	Yamaha 4-op FM
GRAVISULTRA_DESC=	Gravis Ultrasound
SBAWE32_DESC=	SB AWE32 Synth

.include <bsd.port.options.mk>

post-patch:
	${REINPLACE_CMD} '/^read/d' ${WRKSRC}/Configure

.if ${PORT_OPTIONS:MYAMAHA2}
CONFIGURE_ENV+=	ans1=2
.elif ${PORT_OPTIONS:MYAMAHA4}
CONFIGURE_ENV+=	ans1=3
.elif ${PORT_OPTIONS:MGRAVISULTRA}
CONFIGURE_ENV+=	ans1=4
.elif ${PORT_OPTIONS:MSBAWE32}
CONFIGURE_ENV+=	ans1=5
.endif # EXTERNAL is default anyway; just use other defaults

.include <bsd.port.mk>
